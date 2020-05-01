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

MainFrame::MainFrame(const std::wstring &URL) : m_URL(URL)
{
}

LRESULT MainFrame::OnCreate(UINT, WPARAM, LPARAM, BOOL &)
{
    std::wstring caption(L"Login to ");
    caption.append(m_URL);
    ::SetWindowTextW(*this, caption.c_str());

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

bool MainFrame::TranslateAccelerator(LPMSG msg)
{
    return S_FALSE != m_inPlaceObj->TranslateAccelerator(msg);
}
