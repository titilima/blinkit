// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_app.cpp
// Description: WinApp Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_app.h"

#include "base/win/resource_util.h"

#include "blink_impl/win_task_runner.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "blink_impl/win_clipboard.h"
#   include "blink_impl/win_theme_engine.h"
#   include "view/win_view.h"
#endif

namespace BlinKit {

WinApp::WinApp(void)
{
    m_msgHook = SetWindowsHookEx(WH_GETMESSAGE, HookProc, nullptr, threadId());
}

WinApp::~WinApp(void)
{
    UnhookWindowsHookEx(m_msgHook);
}

blink::WebString WinApp::defaultLocale(void)
{
    std::string localName("en-US");

    int len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, nullptr, 0);
    if (len > 0)
    {
        localName.resize(len - 1);
        GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, const_cast<PSTR>(localName.c_str()), len);
    }

    return blink::WebString::fromUTF8(localName);
}

WinApp& WinApp::Get(void)
{
    return static_cast<WinApp &>(AppImpl::Get());
}

LRESULT CALLBACK WinApp::HookProc(int code, WPARAM w, LPARAM l)
{
    WinApp &app = Get();
    do {
        if (code < 0 || PM_REMOVE != w)
            break;

        LPMSG msg = reinterpret_cast<LPMSG>(l);
        static_cast<WinTaskRunner *>(app.taskRunner())->ProcessMessage(*msg);
    } while (false);
    return CallNextHookEx(app.m_msgHook, code, w, l);
}

#ifndef BLINKIT_CRAWLER_ONLY

blink::WebClipboard* WinApp::clipboard(void)
{
    if (!m_clipboard)
    {
        AutoLock lock(m_lock);
        if (!m_clipboard)
            m_clipboard = std::make_unique<WinClipboard>();
    }
    return m_clipboard.get();
}

BkView* BKAPI WinApp::CreateView(BkViewClient &client)
{
    return new WinView(client);
}

blink::WebData WinApp::loadResource(const char *name)
{
    void *data;
    size_t dataLength;
    if (base::GetResourceFromModule(theModule, name, "RES", &data, &dataLength))
        return blink::WebData(reinterpret_cast<const char *>(data), dataLength);

    assert(false); // Failed loading resource!
    return blink::WebData();
}

blink::WebThemeEngine* WinApp::themeEngine(void)
{
    if (!m_themeEngine)
    {
        AutoLock lock(m_lock);
        if (!m_themeEngine)
            m_themeEngine = std::make_unique<WinThemeEngine>();
    }
    return m_themeEngine.get();
}

#endif // BLINKIT_CRAWLER_ONLY

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AppImpl::CreateInstance(void)
{
    new WinApp;
}

} // namespace BlinKit
