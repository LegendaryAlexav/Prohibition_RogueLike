//
// Copyright (c) 2016-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Core/FWCore.h"
#include "Core/FWWindows.h"

namespace fw
{
    // Initialize opengl window on windows, huge chunks taken from nehe
    //    http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/
    // Update to GL 3+ using this info:
    //    https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

    // Public methods

    FWCore::FWCore(uint32 width, uint32 height, bgfx::RendererType::Enum renderType)
        : m_windowClientWidth( width )
        , m_windowClientHeight( height )
    {
    }

    FWCore::~FWCore()
    {
    }

#if WIN32
    FWCore* FWCore::createWindowsCore(uint32 width, uint32 height, bgfx::RendererType::Enum renderType)
    {
        return new FWWindows(width, height, renderType);
    }
#endif
} // namespace fw
