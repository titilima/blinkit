// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: response_impl.h
// Description: Response Base Classes
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_RESPONSE_IMPL_H
#define BLINKIT_BKCOMMON_RESPONSE_IMPL_H

#pragma once

#include <atomic>
#include <vector>
#include "bk_http.h"

class ResponseImpl
{
public:
    virtual void Retain(void) = 0;
    virtual void Release(void) = 0;

    virtual int StatusCode(void) const = 0;
    virtual int GetData(int data, BkBuffer *dst) const = 0;

    virtual int GetHeader(const char *name, BkBuffer *dst) const = 0;

    virtual size_t CookiesCount(void) const = 0;
    virtual int GetCookie(size_t index, BkBuffer *dst) const = 0;
};

namespace BlinKit {

class ResponseBase : public ResponseImpl
{
public:
    virtual ~ResponseBase(void) = default;

    void Release(void) override final;
    int StatusCode(void) const override final { return m_statusCode; }
protected:
    ResponseBase(const std::string &URL);

    int GetData(int data, BkBuffer *dst) const override;

    int m_statusCode = 0;
    std::vector<unsigned char> m_body;
private:
    void Retain(void) override final;

    std::atomic_uint m_refCount;
    std::string m_URL;
};

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_RESPONSE_IMPL_H
