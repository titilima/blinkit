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

#include "base/strings/string_util.h"
#include "bkcommon/bk_strings.h"
#include "bkbase/http/http_response.h"
#include "url/gurl.h"
#include "url/url_constants.h"

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
