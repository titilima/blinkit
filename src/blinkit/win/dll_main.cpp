// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: dll_main.cpp
// Description: DllMain Entry
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

HMODULE theModule = nullptr;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, PVOID)
{
    if (DLL_PROCESS_ATTACH == fdwReason)
        theModule = hinstDLL;
    return TRUE;
}
