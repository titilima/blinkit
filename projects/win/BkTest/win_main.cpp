// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: win_main.cpp
// Description: Application Entry
//      Author: Ziming Li
//     Created: 2018-09-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "main_frame.h"

CComModule _Module;

static int Run(int nShowCmd)
{
    MainFrame *w = MainFrame::CreateInstance();
    if (nullptr == w)
        return EXIT_FAILURE;

    w->ShowWindow(nShowCmd);
    w->UpdateWindow();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int nShowCmd)
{
    _Module.Init(nullptr, hInstance);

    BkApp *app = BkInitialize(nullptr);

    int r = Run(nShowCmd);

    app->Exit();
    _Module.Term();
    return r;
}
