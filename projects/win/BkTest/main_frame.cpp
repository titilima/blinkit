// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: main_frame.cpp
// Description: Main Window Class
//      Author: Ziming Li
//     Created: 2018-09-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "main_frame.h"

MainFrame* MainFrame::CreateInstance(void)
{
    MainFrame *w = new MainFrame;
    assert(nullptr != w);

    if (nullptr != w)
    {
        HWND h = w->Create(nullptr);
        if (nullptr != h)
            return w;

        assert(nullptr != h);
        delete w;
    }

    return nullptr;
}

LRESULT MainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    GetView()->Load("file:///D:/Products/Project1412/webui/main/index.html");
    return 0;
}

LRESULT MainFrame::OnDestroy(UINT, WPARAM, LPARAM, BOOL &)
{
    PostQuitMessage(EXIT_SUCCESS);
    return 0;
}
