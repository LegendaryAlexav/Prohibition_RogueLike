//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "ShaderCompiler.h"

bool IsFile1NewerThanFile2(const char* fullpath1, const char* fullpath2)
{
    namespace fs = std::filesystem;
    std::error_code ec1, ec2;

    fs::file_time_type time1 = fs::last_write_time( fullpath1, ec1 );
    fs::file_time_type time2 = fs::last_write_time( fullpath2, ec2 );

    if( ec1 || ec2 )
        return true; // If either file doesn't exist, consider file1 newer.

    return time1 > time2;
}

int CompileShaders()
{
    bool shadersNeedRebuild = false;

    // Build list of shader files in folder.
    std::vector<std::string> files = fw::getFilesInFolder( "DataRaw/Shaders/", ".frag" );
    std::vector<std::string> vertFiles = fw::getFilesInFolder( "DataRaw/Shaders/", ".vert" );
    files.insert( files.end(), vertFiles.begin(), vertFiles.end() );

    // Check if any shader text file is newer than its compiled binary.
    for( std::string& file : files )
    {
        std::string file1 = std::string("DataRaw/Shaders/") + file;
        std::string file2 = std::string("Data/Shaders/glsl/") + file + ".bin";
        if( IsFile1NewerThanFile2(file1.c_str(), file2.c_str()) )
        {
            shadersNeedRebuild = true;
            break;
        }
    }

    // Rebuild all shaders if needed.
    if( shadersNeedRebuild )
    {
        SetCurrentDirectory( "DataRaw" );
        int errorCode = std::system( "BuildShaders.bat" );
        SetCurrentDirectory( ".." );
        return errorCode;
    }

    return 0;
}
