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

#include "./win_app.h"

#include "blinkit/app/app_caller_impl.h"
#include "blinkit/win/client_caller_store.h"
#include "blinkit/win/message_loop.h"
#include "third_party/zed/include/zed/string/conv.hpp"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/impl/win_clipboard.h"
#   include "blinkit/blink/impl/win_theme_engine.h"
#   include "blinkit/blink/renderer/wtf/MainThread.h"
#   include "third_party/zed/include/zed/win/hmodule.hpp"
#endif

#pragma comment(lib, "Shlwapi.lib")
#ifdef BLINKIT_UI_ENABLED
#   pragma comment(lib, "Imm32.lib")
#   pragma comment(lib, "Usp10.lib")
#   pragma comment(lib, "UxTheme.lib")
#   pragma comment(lib, "Winmm.lib")
#endif

using namespace blink;

namespace BlinKit {

struct BackgoundModeParams {
    WinApp *app;
    HANDLE hEvent;
};

WinApp::WinApp(BkAppClient *client) : AppImpl(client)
{
}

WinApp::~WinApp(void)
{
    if (nullptr != m_appThread)
        CloseHandle(m_appThread);
}

ClientCaller& WinApp::AcquireCallerForClient(void)
{
    ASSERT(m_clientCallerStore);
    return m_clientCallerStore->Acquire();
}

DWORD WINAPI WinApp::BackgroundThread(PVOID param)
{
#ifndef NDEBUG
    Thread::SetName("BlinKit Thread");
#endif

    BackgoundModeParams *params = reinterpret_cast<BackgoundModeParams *>(param);

    std::unique_ptr<WinApp> app(params->app);
    app->Initialize();
    app->m_appCaller = std::make_unique<AppCallerImpl>(app->taskRunner());
    app->m_clientCallerStore = std::make_unique<ClientCallerStoreImpl>();
    SetEvent(params->hEvent);

    return app->RunMessageLoop();
}

WTF::String WinApp::defaultLocale(void)
{
    std::string localName("en-US");

    int len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, nullptr, 0);
    if (len > 0)
    {
        localName.resize(len - 1);
        GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, const_cast<PSTR>(localName.c_str()), len);
    }

    return WTF::String::fromStdUTF8(localName);
}

void WinApp::Exit(int code)
{
    const DWORD tid = threadId();
    PostThreadMessage(tid, WM_QUIT, code, 0);
    if (nullptr != m_appThread)
    {
        ASSERT(GetCurrentThreadId() != tid);
        WaitForSingleObject(m_appThread, INFINITE);
    }
}

WinApp& WinApp::Get(void)
{
    return static_cast<WinApp &>(AppImpl::Get());
}

void WinApp::Initialize(void)
{
    AppImpl::Initialize();
    m_messageLoop = std::make_unique<MessageLoop>();
    IsGUIThread(TRUE);
}

bool WinApp::InitializeForBackgroundMode(void)
{
    BackgoundModeParams params;
    params.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (nullptr == params.hEvent)
        return false;

    params.app = this;
    m_appThread = CreateThread(nullptr, 0, BackgroundThread, &params, 0, nullptr);
    WaitForSingleObject(params.hEvent, INFINITE);
    CloseHandle(params.hEvent);
    return true;
}

int WinApp::RunMessageLoop(void)
{
    int r = m_messageLoop->Run();
    OnExit();
    return r;
}

std::shared_ptr<blink::WebTaskRunner> WinApp::taskRunner(void)
{
    return m_messageLoop->GetTaskRunner();
}

#ifdef BLINKIT_UI_ENABLED
WebClipboard* WinApp::clipboard(void)
{
    ASSERT(isMainThread());
    if (!m_clipboard)
        m_clipboard = std::make_unique<WinClipboard>();
    return m_clipboard.get();
}

WebData WinApp::loadResource(const char *name)
{
    static PCSTR RT_HTMLA = MAKEINTRESOURCEA(23);

    zed::hmodule::resource_data data;
    if (!zed::hmodule::get_resource_data(data, theModule, RT_HTMLA, name))
    {
        ASSERT(false); // Resource not found!
        return AppImpl::loadResource(name);
    }
    return WebData(reinterpret_cast<const char *>(std::get<0>(data)), std::get<1>(data));
}

WebThemeEngine* WinApp::themeEngine(void)
{
    ASSERT(isMainThread());
    if (!m_themeEngine)
        m_themeEngine = std::make_unique<WinThemeEngine>();
    return m_themeEngine.get();
}
#endif // BLINKIT_UI_ENABLED

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef BLINKIT_CRAWLER_ONLY
std::unique_ptr<AppImpl> AppImpl::CreateInstanceForExclusiveMode(BkAppClient *client)
{
    WinApp *app = new WinApp(client);
    if (nullptr != app)
    {
        app->Initialize();
        app->m_appCaller = std::make_unique<SyncAppCallerImpl>();
        app->m_clientCallerStore = std::make_unique<SingletonClientCallerStore>();
    }
    return base::WrapUnique(app);
}
#endif

bool AppImpl::InitializeForBackgroundMode(BkAppClient *client)
{
    WinApp *app = new WinApp(client);
    if (nullptr == app)
    {
        ASSERT(nullptr != app);
        return false;
    }

    return app->InitializeForBackgroundMode();
}

void AppImpl::Log(const char *s)
{
    std::wstring ws = zed::multi_byte_to_wide_string(s);
    ws.append(L"\r\n");
    ::OutputDebugStringW(ws.c_str());
}

} // namespace BlinKit
