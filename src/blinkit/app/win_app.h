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

#include "blinkit/app/app_impl.h"

namespace BlinKit {

class WinClipboard;
class WinSingleThreadTaskRunner;
class WinThemeEngine;

class WinApp final : public AppImpl
{
public:
    WinApp(HANDLE hBackgroundThread = nullptr);
    ~WinApp(void) override;

    static WinApp& Get(void);
private:
    friend class AppImpl;

    static LRESULT CALLBACK HookProc(int code, WPARAM w, LPARAM l);
    static DWORD WINAPI BackgroundThread(PVOID param);

    // blink::Platform
    WTF::String DefaultLocale(void) override;
    // blink::Thread
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
    // AppImpl
    bool IsBackgroundMode(void) const override { return nullptr != m_backgroundThread; }
    void FinalizeInBackground(void) override;

    const DWORD m_appThreadId;
    HHOOK m_msgHook = nullptr;
    HANDLE m_backgroundThread;
    std::shared_ptr<WinSingleThreadTaskRunner> m_taskRunner;

#if 0 // BKTODO:
#ifndef BLINKIT_CRAWLER_ONLY
    // BkApp
    BkView* BKAPI CreateView(BkViewClient &client) override;
    // blink::Platform
    blink::WebClipboard* clipboard(void) override;
    blink::WebThemeEngine* themeEngine(void) override;
    blink::WebData loadResource(const char *name) override;

    std::unique_ptr<WinClipboard> m_clipboard;
    std::unique_ptr<WinThemeEngine> m_themeEngine;
#endif
#endif // 0
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_APP_H
