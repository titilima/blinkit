// -------------------------------------------------
// BlinKit - Login Helper
// -------------------------------------------------
//   File Name: win_main.cpp
// Description: Application Entry
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include <atlbase.h>
#include <WinInet.h>
#include "main_frame.h"
#include "base\strings\string_util.h"
#include "base\strings\sys_string_conversions.h"

struct RunParams {
    std::wstring URL;
    std::string Proxy;
};

CComModule _Module;

static const WCHAR EnableFeatureParam[] = L"-EnableBrowserFeature";
static const TCHAR FeatureKey[] = _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION");
static const DWORD IE9FeatureData = 9999;

static int EnableBrowserFeature(void)
{
    HKEY hKey = nullptr;
    LONG lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, FeatureKey, 0, KEY_SET_VALUE, &hKey);
    if (ERROR_SUCCESS != lr)
    {
        ATLASSERT(ERROR_SUCCESS == lr);
        return EXIT_FAILURE;
    }

    WCHAR path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
    PCWSTR appName = wcsrchr(path, L'\\') + 1;
    DWORD dwData = IE9FeatureData;
    lr = RegSetValueEx(hKey, appName, 0, REG_DWORD, reinterpret_cast<const BYTE *>(&dwData), sizeof(DWORD));

    RegCloseKey(hKey);
    return ERROR_SUCCESS == lr ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void EnsureBrowserFeature(void)
{
    HKEY hKey = nullptr;
    LONG lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, FeatureKey, 0, KEY_QUERY_VALUE, &hKey);
    if (ERROR_SUCCESS != lr)
    {
        ATLASSERT(ERROR_SUCCESS == lr);
        return;
    }

    WCHAR path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));

    do {
        PCWSTR appName = wcsrchr(path, L'\\') + 1;
        lr = RegQueryValueExW(hKey, appName, nullptr, nullptr, nullptr, nullptr);
        if (ERROR_SUCCESS == lr)
            break;

        SHELLEXECUTEINFOW sei = { 0 };
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.lpVerb = L"runas";
        sei.lpFile = path;
        sei.lpParameters = EnableFeatureParam;
        sei.nShow = SW_HIDE;
        ShellExecuteExW(&sei);
        if (nullptr != sei.hProcess)
        {
            WaitForSingleObject(sei.hProcess, INFINITE);
            CloseHandle(sei.hProcess);
        }
    } while (false);
    RegCloseKey(hKey);
}

static void GetRunParams(RunParams &dst, PCWSTR lpCmdLine)
{
    if (nullptr == lpCmdLine || L'\0' == *lpCmdLine)
    {
        ATLASSERT(false); // TODO:
    }
    else
    {
        PCWSTR p = wcschr(lpCmdLine, ' ');
        ATLASSERT(nullptr != p);
        dst.URL.assign(lpCmdLine, p - lpCmdLine);
        base::TrimString(dst.URL, base::kWhitespaceUTF16, &dst.URL);
        dst.Proxy = base::SysWideToUTF8(p + 1);
        base::TrimString(dst.Proxy, base::kWhitespaceASCII, &dst.Proxy);
    }
}

static int RunMainFrame(const std::wstring &URL, int nShowCmd)
{
    MainFrame mainFrame(URL);
    if (!mainFrame.Create(nullptr))
        return EXIT_FAILURE;

    mainFrame.ShowWindow(nShowCmd);
    mainFrame.UpdateWindow();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!mainFrame.TranslateAccelerator(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nShowCmd)
{
    if (0 == _wcsicmp(lpCmdLine, EnableFeatureParam))
        return EnableBrowserFeature();
    EnsureBrowserFeature();

    CoInitialize(nullptr);
    _Module.Init(nullptr, hInstance);

    RunParams params;
    GetRunParams(params, lpCmdLine);

    INTERNET_PROXY_INFO proxyInfo = { 0 };
    proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
    proxyInfo.lpszProxy = reinterpret_cast<PCTSTR>(params.Proxy.c_str());
    InternetSetOptionA(nullptr, INTERNET_OPTION_PROXY, &proxyInfo, sizeof(INTERNET_PROXY_INFO));

    int r = RunMainFrame(params.URL, nShowCmd);

    _Module.Term();
    CoUninitialize();
    return r;
}
