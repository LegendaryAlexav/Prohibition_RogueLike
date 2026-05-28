//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Uniforms.h"

namespace fw
{
    Uniforms::Uniforms()
    {
    }

    Uniforms::~Uniforms()
    {
        for( auto& uniformPair : m_map )
        {
            bgfx::destroy( uniformPair.second );
        }
    }

    void Uniforms::createUniform(const char* name, bgfx::UniformType::Enum type, int arraySize)
    {
        m_map[name] = bgfx::createUniform( name, type, arraySize );
    }

    void Uniforms::set(const char* name, float value, int arrayElement)
    {
        if( m_valuesVec4[name].size() <= arrayElement )
            m_valuesVec4[name].resize( arrayElement + 1 );
        m_valuesVec4[name][arrayElement] = value;
    }

    void Uniforms::set(const char* name, vec2 value, int arrayElement)
    {
        if( m_valuesVec4[name].size() <= arrayElement )
            m_valuesVec4[name].resize( arrayElement + 1 );
        m_valuesVec4[name][arrayElement] = vec4( value, 0, 0 );
    }

    void Uniforms::set(const char* name, vec3 value, int arrayElement)
    {
        if( m_valuesVec4[name].size() <= arrayElement )
            m_valuesVec4[name].resize( arrayElement + 1 );
        m_valuesVec4[name][arrayElement] = vec4( value, 0 );
    }

    void Uniforms::set(const char* name, vec4 value, int arrayElement)
    {
        if( m_valuesVec4[name].size() <= arrayElement )
            m_valuesVec4[name].resize( arrayElement + 1 );
        m_valuesVec4[name][arrayElement] = value;
    }

    void Uniforms::set(const char* name, color4f value, int arrayElement)
    {
        if( m_valuesVec4[name].size() <= arrayElement )
            m_valuesVec4[name].resize( arrayElement + 1 );
        m_valuesVec4[name][arrayElement] = vec4( value.r, value.g, value.b, value.a );
    }

    void Uniforms::set(const char* name, mat4 value, int arrayElement)
    {
        if( m_valuesMat4[name].size() <= arrayElement )
            m_valuesMat4[name].resize( arrayElement + 1 );
        m_valuesMat4[name][arrayElement] = value;
    }

    void Uniforms::set(const char* name, int value, int arrayElement)
    {
        if( m_valuesInt[name].size() <= arrayElement )
            m_valuesInt[name].resize( arrayElement + 1 );
        m_valuesInt[name][arrayElement] = value;
    }

    void Uniforms::setUniforms() const
    {
        for( auto& pair : m_map )
        {
            bgfx::UniformHandle uniformHandle = pair.second;
            bgfx::UniformInfo info;
            bgfx::getUniformInfo( uniformHandle, info );

            if( info.type == bgfx::UniformType::Vec4 )
            {
                if( m_valuesVec4.count(pair.first) )
                {
                    const std::vector<vec4>& values = m_valuesVec4.at( pair.first );
                    bgfx::setUniform( uniformHandle, values.data(), (int)values.size() );
                }
            }
            if( info.type == bgfx::UniformType::Mat4 )
            {
                if( m_valuesMat4.count(pair.first) )
                {
                    const std::vector<mat4>& values = m_valuesMat4.at( pair.first );
                    bgfx::setUniform( uniformHandle, values.data(), (int)values.size() );
                }
            }
            if( info.type == bgfx::UniformType::Sampler )
            {
                if( m_valuesInt.count(pair.first) )
                {
                    const std::vector<int>& values = m_valuesInt.at( pair.first );
                    //bgfx::setUniform( uniformHandle, values.data(), (int)values.size() );
                }
            }
        }
    }
} // namespace fw
