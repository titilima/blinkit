// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: win_request.cpp
// Description: WinRequest Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "win_request.h"

#include "base/strings/string_split.h"

namespace BlinKit {

WinRequest::WinRequest(const char *URL, const BkRequestClient &client)
    : RequestImpl(URL, client)
{
    // Nothing
}

WinRequest::~WinRequest(void)
{
    if (nullptr != m_hThread)
        CloseHandle(m_hThread);
}

std::optional<CURLProxy> WinRequest::GetProxyForCURL(void) const
{
    if (BK_PROXY_SYSTEM_DEFAULT != ProxyType())
        return RequestImpl::GetProxyForCURL();

    std::optional<CURLProxy> ret = std::nullopt;

    HKEY hKey = nullptr;
    do {
        LSTATUS l = RegOpenKeyExA(HKEY_CURRENT_USER,
            R"(Software\Microsoft\Windows\CurrentVersion\Internet Settings)",
            0, KEY_QUERY_VALUE, &hKey);
        if (ERROR_SUCCESS != l)
            break;

        DWORD proxyEnable = 0;
        DWORD cb = sizeof(DWORD);
        l = RegQueryValueExA(hKey, "ProxyEnable", nullptr, nullptr, reinterpret_cast<LPBYTE>(&proxyEnable), &cb);
        if (ERROR_SUCCESS != l || 0 == proxyEnable)
            break;

        static const char ProxyServer[] = "ProxyServer";

        std::string proxyServer(128, '\0');
        cb = proxyServer.length();
        l = RegQueryValueExA(hKey, ProxyServer, nullptr, nullptr,
            reinterpret_cast<LPBYTE>(const_cast<char *>(proxyServer.data())), &cb);
        switch (l)
        {
            case ERROR_SUCCESS:
                proxyServer.resize(cb - 1); // 1 for '\0';
                break;
            case ERROR_MORE_DATA:
                proxyServer.resize(cb - 1); // 1 for '\0';
                l = RegQueryValueExA(hKey, ProxyServer, nullptr, nullptr,
                    reinterpret_cast<LPBYTE>(const_cast<char *>(proxyServer.data())), &cb);
                break;
            default:
                NOTREACHED();
        }

        if (ERROR_SUCCESS != l)
            break;

        ret = ParseProxyForCURL(proxyServer);
    } while (false);

    if (nullptr != hKey)
        RegCloseKey(hKey);
    return ret;
}

std::optional<CURLProxy> WinRequest::ParseProxyForCURL(const std::string &proxy) const
{
    std::vector<std::string> parts = base::SplitString(proxy, ";", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    for (const std::string &part : parts)
    {
        size_t p = part.find('=');
        if (std::string::npos == p)
            return std::make_pair(CURLPROXY_HTTP, part);

        std::string scheme = part.substr(0, p);
        if (CurrentURL().scheme_is(scheme.c_str()))
            return std::make_pair(CURLPROXY_HTTP, part.substr(p + 1));
    }
    return std::nullopt;
}

bool WinRequest::StartWorkThread(void)
{
    m_hThread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
    return nullptr != m_hThread;
}

DWORD WINAPI WinRequest::ThreadProc(PVOID param)
{
    reinterpret_cast<WinRequest *>(param)->DoThreadWork();
    return EXIT_SUCCESS;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RequestImpl* RequestImpl::CreateInstance(const char *URL, const BkRequestClient &client)
{
    return new BlinKit::WinRequest(URL, client);
}
