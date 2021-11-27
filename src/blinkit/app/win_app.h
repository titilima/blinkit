#pragma once
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

#include "blinkit/app/app_impl.h"

namespace BlinKit {

class MessageLoop;
#ifdef BLINKIT_UI_ENABLED
class WinContextMenuController;
class WinThemeEngine;
#endif

class WinApp final : public AppImpl
{
public:
    WinApp(BkAppClient *client);
    ~WinApp(void) override;

    static WinApp& Get(void);

    WinContextMenuController& ContextMenuController(void) { return *m_contextMenuController; }

    int RunMessageLoop(BkMessageFilter filter, void *userData);
private:
    // blink::Platform
    String defaultLocale(void) override;
#ifdef BLINKIT_UI_ENABLED
    blink::WebClipboard* clipboard(void) override;
    // Returns a blob of data corresponding to the named resource.
    blink::WebThemeEngine* themeEngine(void) override;
    blink::WebData loadResource(const char *name) override;
    blink::WebScrollbarBehavior* scrollbarBehavior(void) override;
#endif
    // blink::Thread
    std::shared_ptr<blink::WebTaskRunner> taskRunner(void) override;

    std::unique_ptr<MessageLoop> m_messageLoop;
#ifdef BLINKIT_UI_ENABLED
    std::unique_ptr<WinThemeEngine> m_themeEngine;
    std::unique_ptr<WinContextMenuController> m_contextMenuController;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_APP_H
