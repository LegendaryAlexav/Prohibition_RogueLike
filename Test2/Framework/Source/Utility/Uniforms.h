//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "bgfx/platform.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace fw
{
    class Uniforms
    {
    public:
        Uniforms();
        ~Uniforms();

        void createUniform(const char* name, bgfx::UniformType::Enum type, int arraySize = 1);
        bgfx::UniformHandle getUniform(const char* name) const { return m_map.at(name); }

        void set(const char* name, float value, int arrayElement = 0);
        void set(const char* name, vec2 value, int arrayElement = 0);
        void set(const char* name, vec3 value, int arrayElement = 0);
        void set(const char* name, vec4 value, int arrayElement = 0);
        void set(const char* name, color4f value, int arrayElement = 0);
        void set(const char* name, mat4 value, int arrayElement = 0);
        void set(const char* name, int value, int arrayElement = 0);

        void setUniforms() const;

    protected:
        std::unordered_map<std::string, bgfx::UniformHandle> m_map;
        std::unordered_map<std::string, std::vector<vec4>> m_valuesVec4;
        std::unordered_map<std::string, std::vector<int>> m_valuesInt;
        std::unordered_map<std::string, std::vector<mat4>> m_valuesMat4;
    };
} // namespace fw
