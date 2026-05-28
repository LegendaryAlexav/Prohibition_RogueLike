//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include <filesystem>

namespace fw
{
    void outputMessage(const char* message, ...)
    {
#define MAX_MESSAGE 1024
        char szBuff[MAX_MESSAGE];

        va_list arg;
        va_start(arg, message);
        //vsnprintf_s(szBuff, sizeof(szBuff), _TRUNCATE, message, arg);
        vsnprintf(szBuff, sizeof(szBuff), message, arg);
        va_end(arg);

        szBuff[MAX_MESSAGE - 1] = 0; // vsnprintf_s might do this, but docs are unclear.

#if WIN32
        OutputDebugString(szBuff);
#else
        printf("%s", szBuff);
#endif
    }

    char* loadCompleteFile(const char* filename, uint32* length)
    {
        char* fileContents = 0;

        FILE* fileHandle;
#if WIN32
        errno_t error = fopen_s( &fileHandle, filename, "rb" );
#else
        fileHandle = fopen( filename, "rb" );
#endif

        if( fileHandle )
        {
            fseek( fileHandle, 0, SEEK_END );
            long size = ftell( fileHandle );
            rewind( fileHandle );

            fileContents = new char[size + 1];
            fread( fileContents, size, 1, fileHandle );
            fileContents[size] = 0;

            if( length )
                *length = size;

            fclose( fileHandle );
        }

        return fileContents; // This pointer needs to be deleted when no longer needed.
    }

    double getSystemTime()
    {
        // Return time in seconds with high precision.
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = std::chrono::duration<double>( now.time_since_epoch() );

        return duration.count();
    }

    double getSystemTimeSinceGameStart()
    {
        static double starttime = -1;

        if( starttime == -1 )
            starttime = getSystemTime();

        return getSystemTime() - starttime;
    }

    std::vector<std::string> getFilesInFolder(std::string path, std::string extension)
    {
        std::vector<std::string> files;

        for( const auto& entry : std::filesystem::directory_iterator(path) )
        {
            std::string filename = entry.path().filename().string();
            if( filename.find(extension) != std::string::npos )
            {
                files.push_back( filename );
            }
        }

        return files;
    }

} // namespace fw
