// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: posix_request.h
// Description: PosixRequest Class
//      Author: Ziming Li
//     Created: 2019-09-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKBASE_POSIX_REQUEST_H
#define BLINKIT_BKBASE_POSIX_REQUEST_H

#pragma once

#include <pthread.h>
#include "bkbase/http/request_impl.h"

namespace BlinKit {

class PosixRequest final : public RequestImpl
{
public:
    PosixRequest(const char *URL, const BkRequestClient &client);
    ~PosixRequest(void);
private:
    static void* ThreadProc(void *arg);

    // RequestImpl
    bool StartWorkThread(void) override;

    pthread_t m_thread = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BKBASE_POSIX_REQUEST_H
