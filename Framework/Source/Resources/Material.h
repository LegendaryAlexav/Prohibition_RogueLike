//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "ResourceManager.h"

namespace fw
{
    class ShaderProgram;
    class Texture;
    class Uniforms;

    class Material : public Resource
    {
    public:
        enum class BlendEquation // Change c_BlendEquationConversions if new values added.
        {
            None,
            Add,
            Sub,
            RevSub,
            Min,
            Max,
            SameAsColor, // Needs to stay at end of list.
        };

        enum class BlendFunc // Change c_ColorBlendFuncConversions and c_AlphaBlendFuncConversions if new values added.
        {
            Zero,
            One,
            SrcColor,
            InvSrcColor,
            SrcAlpha,
            InvSrcAlpha,
            DstAlpha,
            InvDstAlpha,
            DstColor,
            InvDstColor,
            SrcAlphaSaturate,
            SameAsColor, // Needs to stay at end of list.
        };

        enum RenderStateFlag
        {
            WriteR              = 1 << 0,
            WriteG              = 1 << 1,
            WriteB              = 1 << 2,
            WriteA              = 1 << 3,
            WriteZ              = 1 << 4,
            CullCW              = 1 << 6,
            CullCCW             = 1 << 7,
            WriteRGB            = WriteR + WriteG + WriteB,
            WriteRGBA           = WriteR + WriteG + WriteB + WriteA,
            RenderAsDots = 1 << 8,
        };

        enum class DepthTest // Order must match bgfx order starting at BGFX_STATE_DEPTH_TEST_LESS.
        {
            Less,
            LEqual,
            Equal,
            GEqual,
            Greater,
            NotEqual,
            Never,
            Always,
        };

    public:
        Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha, float specularPower = 100.0f);
        virtual ~Material();

        static ResourceCategoryIDType getResourceCategoryID() { return "Material"; }

        void enable(Uniforms* pUniforms) const;

        // Getters.
        ShaderProgram* getShader() const { return m_pShader; }
        Texture* getTexture() const { return m_pTexture; }
        vec2 getUVScale() const { return m_uvScale; }
        vec2 getUVOffset() const { return m_uvOffset; }
        color4f getColor() const { return m_color; }
        uint32 getRenderStateFlags() const { return m_renderStateFlags; }
        uint64_t getBGFXRenderState() const;
        uint64_t getBGFXAlphaState() const;

        // Setters.
        void setShader(ShaderProgram* pShader) { m_pShader = pShader; }
        void setTexture(Texture* pTexture) { m_pTexture = pTexture; }
        void setUVScale(vec2 uvScale) { m_uvScale = uvScale; }
        void setUVOffset(vec2 uvOffset) { m_uvOffset = uvOffset; }
        void setColor(color4f color) { m_color = color; }
        void setSpecularPower(float specularPower) { m_specularPower = specularPower; }
        void setColorBlend(BlendEquation eq, BlendFunc srcFunc, BlendFunc dstFunc) { m_colorBlendEquation = eq; m_srcColorBlendFunc = srcFunc; m_dstColorBlendFunc = dstFunc; }
        void setAlphaBlend(BlendEquation eq = BlendEquation::SameAsColor, BlendFunc srcFunc = BlendFunc::SameAsColor, BlendFunc dstFunc = BlendFunc::SameAsColor) { m_alphaBlendEquation = eq; m_srcAlphaBlendFunc = srcFunc; m_dstAlphaBlendFunc = dstFunc; }
        void setRenderStateFlags(uint32 flags) { m_renderStateFlags = flags; }
        void setDepthTest(DepthTest setting) { m_depthTest = setting; }

    protected:
        ShaderProgram* m_pShader = nullptr;
        Texture* m_pTexture = nullptr;
        vec2 m_uvScale = { 1, 1 };
        vec2 m_uvOffset = { 0, 0 };
        color4f m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_specularPower = 0.0f;

        BlendEquation m_colorBlendEquation = BlendEquation::Add;
        BlendEquation m_alphaBlendEquation = BlendEquation::SameAsColor;
        BlendFunc m_srcColorBlendFunc = BlendFunc::SrcAlpha;
        BlendFunc m_dstColorBlendFunc = BlendFunc::InvSrcAlpha;
        BlendFunc m_srcAlphaBlendFunc = BlendFunc::SameAsColor;
        BlendFunc m_dstAlphaBlendFunc = BlendFunc::SameAsColor;

        uint32 m_renderStateFlags = RenderStateFlag::WriteRGBA | RenderStateFlag::WriteZ | RenderStateFlag::CullCCW;
        DepthTest m_depthTest = DepthTest::Less;
    };

} // namespace fw
