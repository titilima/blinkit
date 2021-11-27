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

#include "blinkit/win/message_loop.h"
#include "third_party/zed/include/zed/string/conv.hpp"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/impl/win_clipboard.h"
#   include "blinkit/blink/impl/win_scrollbar_behavior.h"
#   include "blinkit/blink/impl/win_theme_engine.h"
#   include "blinkit/blink/renderer/wtf/MainThread.h"
#   include "blinkit/ui/win_context_menu_controller.h"
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

WinApp::WinApp(BkAppClient *client)
    : AppImpl(client)
    , m_messageLoop(std::make_unique<MessageLoop>())
    , m_contextMenuController(std::make_unique<WinContextMenuController>())
{
#ifndef NDEBUG
    Thread::SetName("BlinKit Thread");
#endif
}

WinApp::~WinApp(void) = default;

String WinApp::defaultLocale(void)
{
    std::string localName("en-US");

    int len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, nullptr, 0);
    if (len > 0)
    {
        localName.resize(len - 1);
        GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, const_cast<PSTR>(localName.c_str()), len);
    }

    return String::fromStdUTF8(localName);
}

WinApp& WinApp::Get(void)
{
    return static_cast<WinApp &>(AppImpl::Get());
}

int WinApp::RunMessageLoop(BkMessageFilter filter, void *userData)
{
    int ret = m_messageLoop->Run(filter, userData);
    delete this;
    return ret;
}

std::shared_ptr<blink::WebTaskRunner> WinApp::taskRunner(void)
{
    return m_messageLoop->GetTaskRunner();
}

#ifdef BLINKIT_UI_ENABLED
WebClipboard* WinApp::clipboard(void)
{
    static WinClipboard s_clipboard;
    return &s_clipboard;
}

WebData WinApp::loadResource(const char *name)
{
    static PCSTR RT_HTMLA = MAKEINTRESOURCEA(23);

    zed::hmodule::resource_data data;
    if (zed::hmodule::get_resource_data(data, theModule, RT_HTMLA, name))
        return WebData(reinterpret_cast<const char *>(std::get<0>(data)), std::get<1>(data));
    return AppImpl::loadResource(name);
}

WebScrollbarBehavior* WinApp::scrollbarBehavior(void)
{
    static WinScrollbarBehavior s_scrollbarBehavior;
    return &s_scrollbarBehavior;
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

void AppImpl::Log(const char *s)
{
    std::wstring ws = zed::multi_byte_to_wide_string(s);
    ws.append(L"\r\n");
    ::OutputDebugStringW(ws.c_str());
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace BlinKit;

extern "C" {

BKEXPORT bool_t BKAPI BkInitialize(BkAppClient *client)
{
    if (nullptr != Platform::current())
        return false;
    new WinApp(client);
    return true;
}

static bool_t BKAPI DummyMessageFilter(const MSG *, void *)
{
    return false;
}

BKEXPORT int BKAPI BkRunMessageLoop(void)
{
    return BkRunMessageLoopEx(DummyMessageFilter, nullptr);
}

BKEXPORT int BKAPI BkRunMessageLoopEx(BkMessageFilter filter, void *userData)
{
    return WinApp::Get().RunMessageLoop(filter, userData);
}

#if 0 // BKTODO:
BKEXPORT void BKAPI BkFinalize(void)
{
    Platform *p = Platform::current();
    if (nullptr == p)
        return;

    static_cast<AppImpl *>(p)->Exit(EXIT_SUCCESS);
}
#endif

} // extern "C"
