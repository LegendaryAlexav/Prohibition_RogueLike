//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "bgfx/platform.h"

#include "ShaderProgram.h"
#include "Utility/Utility.h"

namespace fw
{
    ShaderProgram::ShaderProgram()
    {
    }

    ShaderProgram::ShaderProgram(const char* shaderFolder, const char* vertFilename, const char* fragFilename)
    {
        init( shaderFolder, vertFilename, fragFilename );
    }

    ShaderProgram::~ShaderProgram()
    {
        cleanup();
    }

    void ShaderProgram::cleanup()
    {
        if( m_vertShaderString )
            delete[] m_vertShaderString;
        if( m_fragShaderString )
            delete[] m_fragShaderString;

        bgfx::destroy( m_program );
        bgfx::destroy( m_vertShader );
        bgfx::destroy( m_fragShader );

        m_vertShaderString = nullptr;
        m_fragShaderString = nullptr;
    }

    bool ShaderProgram::init(const char* shaderFolder, const char* vertFilename, const char* fragFilename)
    {
#define MAX_PATH 260
        char vertFullPath[MAX_PATH];
        char fragFullPath[MAX_PATH];

        const char* rendererPath = nullptr;

        switch( bgfx::getRendererType() )
        {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: rendererPath = "dx11";  break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:        rendererPath = "pssl";  break;
        case bgfx::RendererType::Metal:      rendererPath = "metal"; break;
        case bgfx::RendererType::Nvn:        rendererPath = "nvn";   break;
        case bgfx::RendererType::OpenGL:     rendererPath = "glsl";  break;
        case bgfx::RendererType::OpenGLES:   rendererPath = "essl";  break;
        case bgfx::RendererType::Vulkan:     rendererPath = "spirv"; break;
        //case bgfx::RendererType::WebGPU:     rendererPath = "fixme"; break;
        case bgfx::RendererType::Count:
            assert( false );
            break;
        }

        snprintf( vertFullPath, MAX_PATH, "%s/%s/%s", shaderFolder, rendererPath, vertFilename );
        snprintf( fragFullPath, MAX_PATH, "%s/%s/%s", shaderFolder, rendererPath, fragFilename );

        m_vertShaderString = loadCompleteFile( vertFullPath, &m_vertShaderStringLength );
        m_fragShaderString = loadCompleteFile( fragFullPath, &m_fragShaderStringLength );

        assert( m_vertShaderString != nullptr && m_fragShaderString != nullptr );
        if( m_vertShaderString == nullptr || m_fragShaderString == nullptr )
            return false;

        return reload();
    }

    bool ShaderProgram::reload()
    {
        assert( m_vertShaderString != nullptr );
        assert( m_fragShaderString != nullptr );

        const bgfx::Memory* vertMemory = bgfx::makeRef( m_vertShaderString, m_vertShaderStringLength );
        const bgfx::Memory* fragMemory = bgfx::makeRef( m_fragShaderString, m_fragShaderStringLength );

        m_vertShader = bgfx::createShader( vertMemory );
        m_fragShader = bgfx::createShader( fragMemory );

        m_program = bgfx::createProgram( m_vertShader, m_fragShader, false );

        return true;
    }

} // namespace fw
