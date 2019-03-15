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
#include "blink_impl/win_theme_engine.h"

namespace BlinKit {

WinApp::WinApp(void)
{
    m_msgHook = SetWindowsHookEx(WH_GETMESSAGE, HookProc, nullptr, threadId());
}

WinApp::~WinApp(void)
{
    UnhookWindowsHookEx(m_msgHook);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AppImpl::CreateInstance(void)
{
    new WinApp;
}

} // namespace BlinKit
