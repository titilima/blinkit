// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_request.h
// Description: PosixRequest Class
//      Author: Ziming Li
//     Created: 2019-09-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "posix_request.h"

namespace BlinKit {

PosixRequest::PosixRequest(const char *URL, const BkRequestClient &client)
    : RequestImpl(URL, client)
{
    // Nothing
}

PosixRequest::~PosixRequest(void) = default;

bool PosixRequest::StartWorkThread(void)
{
    return 0 == pthread_create(&m_thread, nullptr, ThreadProc, this);
}

void* PosixRequest::ThreadProc(void *arg)
{
    reinterpret_cast<PosixRequest *>(arg)->DoThreadWork();
    return nullptr;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RequestImpl* RequestImpl::CreateInstance(const char *URL, const BkRequestClient &client)
{
    return new BlinKit::PosixRequest(URL, client);
}
