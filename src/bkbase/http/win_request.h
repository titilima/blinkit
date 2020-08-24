// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: win_request.h
// Description: WinRequest Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_REQUEST_H
#define BLINKIT_BLINKIT_WIN_REQUEST_H

#pragma once

#include "request_impl.h"

namespace BlinKit {

class WinRequest final : public RequestImpl
{
public:
    WinRequest(const char *URL, const BkRequestClient &client);
    ~WinRequest(void) override;
private:
    std::optional<CURLProxy> ParseProxyForCURL(const std::string &proxy) const;
    static DWORD WINAPI ThreadProc(PVOID param);

    std::optional<CURLProxy> GetProxyForCURL(void) const override;
    bool StartWorkThread(void) override;

    HANDLE m_hThread = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_REQUEST_H
