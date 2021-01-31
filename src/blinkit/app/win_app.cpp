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

#include "base/memory/ptr_util.h"
#include "base/strings/sys_string_conversions.h"
#include "bkcommon/bk_signal.hpp"
#include "blinkit/app/app_caller_impl.h"
#include "blinkit/win/client_caller_store.h"
#include "blinkit/win/message_loop.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "base/win/resource_util.h"
#   include "blinkit/blink_impl/win_theme_engine.h"
#   include "blinkit/ui/win_web_view.h"
#   include "third_party/blink/renderer/platform/fonts/font_cache.h"
#   include "third_party/skia/include/ports/SkTypeface_win.h"
#endif

#if 0 // BKTODO:
#   include "blink_impl/win_clipboard.h"
#endif // 0

using namespace blink;

namespace BlinKit {

struct BackgoundModeParams final : public ScopedSignalWaiter
{
    WinApp *app;
};

WinApp::WinApp(BkAppClient *client) : AppImpl(client)
{
#ifndef BLINKIT_CRAWLER_ONLY
    auto fontMgr = SkFontMgr_New_GDI();
    FontCache::SetFontManager(std::move(fontMgr));
#endif
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
    BackgoundModeParams *params = reinterpret_cast<BackgoundModeParams *>(param);

    WinApp *app = params->app;
    app->Initialize();
    app->m_appCaller = std::make_unique<AppCallerImpl>(app->GetTaskRunner());
    app->m_clientCallerStore = std::make_unique<ClientCallerStoreImpl>();
    params->Signal();

    return app->RunMessageLoop();
}

WTF::String WinApp::DefaultLocale(void)
{
    std::string localName("en-US");

    int len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, nullptr, 0);
    if (len > 0)
    {
        localName.resize(len - 1);
        GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, const_cast<PSTR>(localName.c_str()), len);
    }

    return WTF::String::FromStdUTF8(localName);
}

void WinApp::Exit(int code)
{
    const DWORD threadId = ThreadId();
    PostThreadMessage(threadId, WM_QUIT, code, 0);
    if (nullptr != m_appThread)
    {
        ASSERT(GetCurrentThreadId() != threadId);
        WaitForSingleObject(m_appThread, INFINITE);
    }
}

WinApp& WinApp::Get(void)
{
    return static_cast<WinApp &>(AppImpl::Get());
}

std::shared_ptr<base::SingleThreadTaskRunner> WinApp::GetTaskRunner(void) const
{
    return m_messageLoop->GetTaskRunner();
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
    if (!params.IsSignalValid())
        return false;

    params.app = this;
    m_appThread = CreateThread(nullptr, 0, BackgroundThread, &params, 0, nullptr);
    return true;
}

int WinApp::RunMessageLoop(void)
{
    int r = m_messageLoop->Run();
    OnExit();
    return r;
}

#ifndef BLINKIT_CRAWLER_ONLY
std::string WinApp::GetDataResource(const char *name)
{
    static const PCSTR RT_HTMLA = MAKEINTRESOURCEA(23);

    void *data;
    size_t size;
    if (!base::GetResourceFromModule(theModule, name, RT_HTMLA, &data, &size))
    {
        ASSERT(false); // Resource not found!
        return AppImpl::GetDataResource(name);
    }
    return std::string(reinterpret_cast<const char *>(data), size);
}

WebThemeEngine* WinApp::ThemeEngine(void)
{
    ASSERT(IsMainThread());
    if (!m_themeEngine)
        m_themeEngine = std::make_unique<WinThemeEngine>();
    return m_themeEngine.get();
}
#endif // BLINKIT_CRAWLER_ONLY

#if 0 // BKTODO:
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
#endif // 0

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
    std::wstring ws = base::SysUTF8ToWide(s);
    ws.append(L"\r\n");
    ::OutputDebugStringW(ws.c_str());
}

} // namespace BlinKit
