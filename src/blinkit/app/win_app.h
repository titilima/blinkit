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
#ifndef BLINKIT_CRAWLER_ONLY
class WinThemeEngine;
#endif

class WinApp final : public AppImpl
{
public:
    WinApp(int mode, BkAppClient *client, HANDLE hBackgroundThread = nullptr);
    ~WinApp(void) override;

    static WinApp& Get(void);
private:
    friend class AppImpl;

    static LRESULT CALLBACK HookProc(int code, WPARAM w, LPARAM l);
    static DWORD WINAPI BackgroundThread(PVOID param);

    // blink::Platform
    WTF::String DefaultLocale(void) override;
#ifndef BLINKIT_CRAWLER_ONLY
    // Returns a blob of data corresponding to the named resource.
    std::string GetDataResource(const char *name) override;
    blink::WebThemeEngine* ThemeEngine(void) override;
#endif
    // blink::Thread
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
    // AppImpl
    void Initialize(BkAppClient *client) override;
    int RunAndFinalize(void) override;
    void Exit(int code) override;

    HHOOK m_msgHook = nullptr;
    HANDLE m_backgroundThread;
    std::shared_ptr<WinSingleThreadTaskRunner> m_taskRunner;
#ifndef BLINKIT_CRAWLER_ONLY
    std::unique_ptr<WinThemeEngine> m_themeEngine;
#endif

#if 0 // BKTODO:
    // blink::Platform
    blink::WebClipboard* clipboard(void) override;

    std::unique_ptr<WinClipboard> m_clipboard;
#endif // 0
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_APP_H
