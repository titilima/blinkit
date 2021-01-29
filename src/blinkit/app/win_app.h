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

class ClientCallerStore;
class MessageLoop;
class WinClipboard;
#ifndef BLINKIT_CRAWLER_ONLY
class WinThemeEngine;
#endif

class WinApp final : public AppImpl
{
public:
    ~WinApp(void) override;

    static WinApp& Get(void);
private:
    friend AppImpl;
    WinApp(BkAppClient *client);

    bool InitializeForBackgroundMode(void);
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
    int RunMessageLoop(void) override;
    void Exit(int code) override;
    ClientCaller& AcquireCallerForClient(void) override;
    void Initialize(void) override;

    HANDLE m_appThread = nullptr; // nullptr for exclusive mode
    std::unique_ptr<MessageLoop> m_messageLoop;
    std::unique_ptr<ClientCallerStore> m_clientCallerStore;
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
