// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: hijack_response.h
// Description: Response Classes for Hijacking
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HIJACK_RESPONSE_H
#define BLINKIT_BLINKIT_HIJACK_RESPONSE_H

#pragma once

#include "bkcommon/response_impl.h"

namespace BlinKit {

class FakeResponse final : public ResponseBase
{
public:
    FakeResponse(const std::string &URL);

    void SetContentType(const char *contentType) { m_contentType = contentType; }
private:
    int GetHeader(const char *name, BkBuffer *dst) const override;
    int EnumerateHeaders(BkHttpHeaderEnumerator enumerator, void *userData) const override;
    size_t CookiesCount(void) const override { return 0; }
    int GetCookie(size_t index, BkBuffer *dst) const override;

    std::string m_contentType;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HIJACK_RESPONSE_H
