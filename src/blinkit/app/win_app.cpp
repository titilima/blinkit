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

#include "blink_impl/win_task_runner.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AppImpl::CreateInstance(void)
{
    new WinApp;
}

} // namespace BlinKit
