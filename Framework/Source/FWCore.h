//
// Copyright (c) 2016-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "bgfx/platform.h"

namespace fw
{
    class GameCore;

    class FWCore
    {
    public:
        FWCore(uint32 width, uint32 height, bgfx::RendererType::Enum renderType);
        virtual ~FWCore();

        bool init(uint32 width, uint32 height);
        uint32 run(GameCore& game);
        void shutdown();

        void setWindowSize(uint32 width, uint32 height);
        void setClientPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized);
        void setWindowPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized);

        bool isKeyDown(uint32 value);
        bool isMouseButtonDown(uint32 id);
        void getMouseCoordinates(int32* mx, int32* my);
        float getMouseWheel();

        HWND getWindowHandle() { return m_hWnd; }
        uint32 getWindowClientWidth() { return m_windowClientWidth; }
        uint32 getWindowClientHeight() { return m_windowClientHeight; }

        void setEscapeKeyWillQuit(bool value) { m_escapeKeyWillQuit = value; }
        void setQuitAtEndOfFrame() { m_quitAtEndOfFrame = true; }

        uint32 getFrameCount() { return m_frameCount; }

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

        uint32 m_windowClientWidth = 0;
        uint32 m_windowClientHeight = 0;

        HWND m_hWnd = nullptr;
        HINSTANCE m_hInstance = nullptr;

        bool m_keyStates[256] = {};
        bool m_mouseButtonStates[3] = {};
        float m_mouseWheel = 0;
        bool m_oldKeyStates[256] = {};
        bool m_oldMouseButtonStates[3] = {};
        bool m_windowIsActive = false;
        bool m_fullscreenMode = false;

        int32 m_frameCount = 0;
    };

} // namespace fw
