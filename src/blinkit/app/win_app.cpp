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

#include "base/strings/sys_string_conversions.h"
#include "blinkit/blink_impl/win_single_thread_task_runner.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "base/win/resource_util.h"
#   include "blinkit/blink_impl/win_theme_engine.h"
#   include "third_party/blink/renderer/platform/fonts/font_cache.h"
#   include "third_party/skia/include/ports/SkTypeface_win.h"
#endif

#if 0 // BKTODO:
#   include "blink_impl/win_clipboard.h"
#endif // 0

using namespace blink;

namespace BlinKit {

struct BackgoundThreadData
{
    BackgoundThreadData(void) : hEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
    {
    }
    ~BackgoundThreadData(void)
    {
        CloseHandle(hEvent);
    }

    HANDLE hEvent;
    BkAppClient *client = nullptr;
    HANDLE hThread = nullptr;
};

WinApp::WinApp(int mode, BkAppClient *client, HANDLE hBackgroundThread)
    : AppImpl(mode, client)
    , m_taskRunner(std::make_shared<WinSingleThreadTaskRunner>())
    , m_backgroundThread(hBackgroundThread)
{
#ifndef BLINKIT_CRAWLER_ONLY
    auto fontMgr = SkFontMgr_New_GDI();
    FontCache::SetFontManager(std::move(fontMgr));
#endif
    m_msgHook = SetWindowsHookEx(WH_GETMESSAGE, HookProc, nullptr, GetCurrentThreadId());
}

WinApp::~WinApp(void)
{
    UnhookWindowsHookEx(m_msgHook);
    if (nullptr != m_backgroundThread)
        CloseHandle(m_backgroundThread);
}

DWORD WINAPI WinApp::BackgroundThread(PVOID param)
{
    BackgoundThreadData *data = reinterpret_cast<BackgoundThreadData *>(param);

    WinApp *app = new WinApp(BK_APP_BACKGROUND_MODE, data->client, data->hThread);
    if (nullptr == app)
    {
        ASSERT(nullptr != app);
        CloseHandle(data->hThread);
        SetEvent(data->hEvent);
        return EXIT_FAILURE;
    }

    app->Initialize(nullptr);
    SetEvent(data->hEvent);

    return app->RunAndFinalize();
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
    DWORD threadId = ThreadId();
    PostThreadMessage(threadId, WM_QUIT, code, 0);
    if (nullptr != m_backgroundThread)
    {
        ASSERT(GetCurrentThreadId() != threadId);
        WaitForSingleObject(m_backgroundThread, INFINITE);
    }
}

WinApp& WinApp::Get(void)
{
    return static_cast<WinApp &>(AppImpl::Get());
}

std::shared_ptr<base::SingleThreadTaskRunner> WinApp::GetTaskRunner(void) const
{
    return m_taskRunner;
}

LRESULT CALLBACK WinApp::HookProc(int code, WPARAM w, LPARAM l)
{
    WinApp &app = Get();
    do {
        if (code < 0 || PM_REMOVE != w)
            break;

        LPMSG msg = reinterpret_cast<LPMSG>(l);
        app.m_taskRunner->ProcessMessage(*msg);
    } while (false);
    return CallNextHookEx(app.m_msgHook, code, w, l);
}

void WinApp::Initialize(BkAppClient *client)
{
    AppImpl::Initialize(client);
    IsGUIThread(TRUE);
}

int WinApp::RunAndFinalize(void)
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    delete this;
    return msg.wParam;
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

AppImpl* AppImpl::CreateInstance(int mode, BkAppClient *client)
{
    return new WinApp(mode, client);
}

void AppImpl::InitializeBackgroundInstance(BkAppClient *client)
{
    BackgoundThreadData data;
    data.client = client;
    data.hThread = CreateThread(nullptr, 0, WinApp::BackgroundThread, &data, 0, nullptr);
    WaitForSingleObject(data.hEvent, INFINITE);
}

void AppImpl::Log(const char *s)
{
    std::wstring ws = base::SysUTF8ToWide(s);
    ws.append(L"\r\n");
    ::OutputDebugStringW(ws.c_str());
}

} // namespace BlinKit
