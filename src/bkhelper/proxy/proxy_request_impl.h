// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_request_impl.h
// Description: ProxyRequestImpl Class
//      Author: Ziming Li
//     Created: 2020-06-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_PROXY_REQUEST_IMPL_H
#define BLINKIT_BKHELPER_PROXY_REQUEST_IMPL_H

#pragma once

#include "bk_def.h"

class ProxyRequestImpl
{
public:
    virtual ~ProxyRequestImpl(void) = default;

    virtual int GetData(int data, BkBuffer *dst) = 0;

    virtual int GetHeader(const char *name, BkBuffer *dst) = 0;
    virtual void SetHeader(const char *name, const char *value) = 0;
};

#endif // BLINKIT_BKHELPER_PROXY_REQUEST_IMPL_H
