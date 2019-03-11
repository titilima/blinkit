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

CComModule _Module;

static int Run(int nShowCmd)
{
    return EXIT_SUCCESS;
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
