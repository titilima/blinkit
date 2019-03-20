// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_app.h
// Description: WinApp Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_APP_H
#define BLINKIT_BLINKIT_WIN_APP_H

#pragma once

#include "app_impl.h"

namespace BlinKit {

class WinClipboard;
class WinThemeEngine;

class WinApp final : public AppImpl
{
public:
    WinApp(void);
    ~WinApp(void);

    static WinApp& Get(void);
private:
    static LRESULT CALLBACK HookProc(int code, WPARAM w, LPARAM l);
    // blink::Platform
    blink::WebClipboard* clipboard(void) override;
    blink::WebThemeEngine* themeEngine(void) override;
    blink::WebData loadResource(const char *name) override;

    std::unique_ptr<WinClipboard> m_clipboard;
    std::unique_ptr<WinThemeEngine> m_themeEngine;
    HHOOK m_msgHook;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_APP_H
