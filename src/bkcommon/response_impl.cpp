// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: response_impl.cpp
// Description: Response Base Classes
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "response_impl.h"

#include "bkcommon/buffer_impl.hpp"

namespace BlinKit {

ResponseBase::ResponseBase(const std::string &URL) : m_refCount(1), m_URL(URL)
{
}

int ResponseBase::GetData(int data, BkBuffer *dst) const
{
    switch (data)
    {
        case BK_RESPONSE_CURRENT_URL:
        case BK_RESPONSE_ORIGINAL_URL:
            BufferImpl::Set(dst, m_URL);
            break;
        case BK_RESPONSE_BODY:
            BufferImpl::Set(dst, m_body);
            break;
        default:
            NOTREACHED();
            return BK_ERR_NOT_FOUND;
    }
    return BK_ERR_SUCCESS;
}

void ResponseBase::HijackBody(const void *newBody, size_t length)
{
    m_body.resize(length);
    if (length > 0)
        memcpy(m_body.data(), newBody, length);
}

void ResponseBase::Release(void)
{
    if (0 == --m_refCount)
        delete this;
}

ResponseImpl* ResponseBase::Retain(void)
{
    ++m_refCount;
    return this;
}

} // namespace BlinKit
