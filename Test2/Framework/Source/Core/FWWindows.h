// Copyright (c) 2016-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#if WIN32
#include "DataTypes.h"

#include "bgfx/platform.h"
#include "Core/FWCore.h"

namespace fw
{
    class GameCore;

    class FWWindows : public FWCore
    {
    public:
        FWWindows(uint32 width, uint32 height, bgfx::RendererType::Enum renderType);
        virtual ~FWWindows();

        virtual bool init(uint32 width, uint32 height) override;
        virtual uint32 run(GameCore& game) override;
        virtual void shutdown() override;

        virtual void setWindowSize(uint32 width, uint32 height) override;
        virtual void setClientPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized) override;
        virtual void setWindowPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized) override;

        virtual bool isKeyDown(uint32 value) override;
        virtual bool isMouseButtonDown(uint32 id) override;
        virtual void getMouseCoordinates(int32* mx, int32* my) override;
        virtual float getMouseWheel() override;

        HWND getWindowHandle() { return m_hWnd; }
        //uint32 getWindowClientWidth() { return m_windowClientWidth; }
        //uint32 getWindowClientHeight() { return m_windowClientHeight; }

        //void setEscapeKeyWillQuit(bool value) { m_escapeKeyWillQuit = value; }
        //void setQuitAtEndOfFrame() { m_quitAtEndOfFrame = true; }

        //uint32 getFrameCount() { return m_frameCount; }

    protected:
        void resizeWindow(uint32 width, uint32 height);
        bool createRenderWindow(const char* title, uint32 width, uint32 height, uint8 colorBits, bool fullscreenflag);
        bool failAndCleanup(const char* pMessage);
        void destroyRenderWindow(bool destroyInstance);

        static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        GameCore* m_pGame = nullptr;

        bool m_escapeKeyWillQuit = true;
        bool m_quitAtEndOfFrame = false;

        HWND m_hWnd = nullptr;
        HINSTANCE m_hInstance = nullptr;
    };
} // namespace fw
#endif
