// -------------------------------------------------
// BlinKit - Login Helper
// -------------------------------------------------
//   File Name: main_frame.cpp
// Description: MainFrame Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "main_frame.h"

MainFrame::MainFrame(const std::wstring &URL, bool useProxy) : m_URL(URL), m_useProxy(useProxy)
{
}

LRESULT MainFrame::OnCreate(UINT, WPARAM, LPARAM, BOOL &)
{
    SetCaption();

    HRESULT hr = CreateControl(L"{8856F961-340A-11D0-A96B-00C04FD705A2}");
    if (FAILED(hr))
        return -1;

    CComPtr<IWebBrowser2> wb2;
    hr = QueryControl(&wb2);
    if (FAILED(hr))
        return -1;

    wb2->QueryInterface(&m_inPlaceObj);
    ATLASSERT(nullptr != m_inPlaceObj);

    CComVariant v(m_URL.c_str());
    wb2->Navigate2(&v, nullptr, nullptr, nullptr, nullptr);
    return 0;
}

LRESULT MainFrame::OnDestroy(UINT, WPARAM, LPARAM, BOOL &)
{
    m_inPlaceObj->Release();
    PostQuitMessage(EXIT_SUCCESS);
    return 0;
}

void MainFrame::SetCaption(void)
{
    size_t p = m_URL.find(L"://");
    std::wstring caption = m_URL.substr(p + 3);
    p = caption.find(L'/');
    if (std::wstring::npos != p)
        caption.resize(p);
    caption.insert(0, m_useProxy ? L"Login to " : L"Navigating ");
    ::SetWindowTextW(*this, caption.c_str());
}

bool MainFrame::TranslateAccelerator(LPMSG msg)
{
    return S_FALSE != m_inPlaceObj->TranslateAccelerator(msg);
}
