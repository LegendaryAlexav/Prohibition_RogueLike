//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Material.h"
#include "Resources/Texture.h"
#include "Utility/Uniforms.h"

namespace fw
{
    Material::Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha, float specularPower)
        : m_pShader( pShader )
        , m_pTexture( pTexture )
        , m_color( color )
        , m_specularPower( specularPower )
    {
        if( hasAlpha == false )
        {
            m_colorBlendEquation = BlendEquation::None;
        }
    }

    Material::~Material()
    {
    }

    void Material::enable(Uniforms* pUniforms) const
    {
        // Textures.
        if( m_pTexture )
        {
            int textureSlot = 0;
            pUniforms->set( "u_textureColor", textureSlot );
            bgfx::setTexture( 0, pUniforms->getUniform("u_textureColor"), m_pTexture->getHandle() );
        }

        // UV scale and offset.
        pUniforms->set( "u_uvScale", m_uvScale );
        pUniforms->set( "u_uvOffset", m_uvOffset );

        // Vertex colors.
        pUniforms->set( "u_diffuseColor", m_color );

        // Lighting.
        pUniforms->set( "u_specularPower", m_specularPower );
    }

    uint64_t c_BlendEquationConversions[6] =
    {
        0,
        BGFX_STATE_BLEND_EQUATION_ADD,
        BGFX_STATE_BLEND_EQUATION_SUB,
        BGFX_STATE_BLEND_EQUATION_REVSUB,
        BGFX_STATE_BLEND_EQUATION_MIN,
        BGFX_STATE_BLEND_EQUATION_MAX,
    };

    uint64_t c_BlendFuncConversions[11] =
    {
        BGFX_STATE_BLEND_ZERO,
        BGFX_STATE_BLEND_ONE,
        BGFX_STATE_BLEND_SRC_COLOR,
        BGFX_STATE_BLEND_INV_SRC_COLOR,
        BGFX_STATE_BLEND_SRC_ALPHA,
        BGFX_STATE_BLEND_INV_SRC_ALPHA,
        BGFX_STATE_BLEND_DST_ALPHA,
        BGFX_STATE_BLEND_INV_DST_ALPHA,
        BGFX_STATE_BLEND_DST_COLOR,
        BGFX_STATE_BLEND_INV_DST_COLOR,
        BGFX_STATE_BLEND_SRC_ALPHA_SAT,
    };

    uint64_t Material::getBGFXRenderState() const
    {
        uint64_t bgfxRenderState = 0;
    
        if( m_renderStateFlags & RenderStateFlag::WriteR ) bgfxRenderState |= BGFX_STATE_WRITE_R;
        if( m_renderStateFlags & RenderStateFlag::WriteG ) bgfxRenderState |= BGFX_STATE_WRITE_G;
        if( m_renderStateFlags & RenderStateFlag::WriteB ) bgfxRenderState |= BGFX_STATE_WRITE_B;
        if( m_renderStateFlags & RenderStateFlag::WriteA ) bgfxRenderState |= BGFX_STATE_WRITE_A;
        if( m_renderStateFlags & RenderStateFlag::WriteZ ) bgfxRenderState |= BGFX_STATE_WRITE_Z;
        if( m_renderStateFlags & RenderStateFlag::CullCW ) bgfxRenderState |= BGFX_STATE_CULL_CW;
        if( m_renderStateFlags & RenderStateFlag::CullCCW ) bgfxRenderState |= BGFX_STATE_CULL_CCW;

        if( m_renderStateFlags & RenderStateFlag::RenderAsDots)
            bgfxRenderState |= BGFX_STATE_PT_POINTS | BGFX_STATE_POINT_SIZE(10);

        bgfxRenderState |= (uint64)BGFX_STATE_DEPTH_TEST_LESS + (uint64)m_depthTest;

        bgfxRenderState |= getBGFXAlphaState();

        return bgfxRenderState;
    }

    uint64_t Material::getBGFXAlphaState() const
    {
        if( m_colorBlendEquation == BlendEquation::None )
            return 0;

        uint64_t bgfxBlendState = 0;

        if( m_alphaBlendEquation == BlendEquation::SameAsColor )
        {
            assert( m_colorBlendEquation < BlendEquation::SameAsColor );

            uint64_t colorBlendEquation = c_BlendEquationConversions[(int)m_colorBlendEquation];
            uint64_t srcColorBlendFunc = c_BlendFuncConversions[(int)m_srcColorBlendFunc];
            uint64_t dstColorBlendFunc = c_BlendFuncConversions[(int)m_dstColorBlendFunc];

            bgfxBlendState = BGFX_STATE_BLEND_EQUATION( colorBlendEquation ) | BGFX_STATE_BLEND_FUNC( srcColorBlendFunc, dstColorBlendFunc );
        }
        else
        {
            assert( m_colorBlendEquation < BlendEquation::SameAsColor );
            assert( m_alphaBlendEquation < BlendEquation::SameAsColor );

            uint64_t colorBlendEquation = c_BlendEquationConversions[(int)m_colorBlendEquation];
            uint64_t alphaBlendEquation = c_BlendEquationConversions[(int)m_alphaBlendEquation];
            uint64_t srcColorBlendFunc = c_BlendFuncConversions[(int)m_srcColorBlendFunc];
            uint64_t dstColorBlendFunc = c_BlendFuncConversions[(int)m_dstColorBlendFunc];
            uint64_t srcAlphaBlendFunc = c_BlendFuncConversions[(int)m_srcAlphaBlendFunc];
            uint64_t dstAlphaBlendFunc = c_BlendFuncConversions[(int)m_dstAlphaBlendFunc];

            bgfxBlendState = BGFX_STATE_BLEND_EQUATION_SEPARATE( colorBlendEquation, alphaBlendEquation )
                | BGFX_STATE_BLEND_FUNC_SEPARATE( srcColorBlendFunc, srcAlphaBlendFunc, dstColorBlendFunc, dstAlphaBlendFunc );
        }

        return bgfxBlendState;
    }

} // namespace fw
