// -------------------------------------------------
// BlinKit - Login Helper
// -------------------------------------------------
//   File Name: main_frame.h
// Description: MainFrame Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LOGIN_HELPER_MAIN_FRAME_H
#define BLINKIT_LOGIN_HELPER_MAIN_FRAME_H

#pragma once

#include <string>
#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0> MainFrameTraits;

class MainFrame : public CWindowImpl<MainFrame, CAxWindow, MainFrameTraits>
{
public:
    MainFrame(const std::wstring &URL, bool useProxy);

    bool TranslateAccelerator(LPMSG msg);
private:
    void SetCaption(void);

    std::wstring m_URL;
    bool m_useProxy;
    IOleInPlaceActiveObject *m_inPlaceObj = nullptr;
public:
    BEGIN_MSG_MAP(MainFrame)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
    END_MSG_MAP()
private:
    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL &);
    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL &);
};

#endif // BLINKIT_LOGIN_HELPER_MAIN_FRAME_H
