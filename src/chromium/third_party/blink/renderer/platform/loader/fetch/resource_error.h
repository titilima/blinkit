// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_error.h
// Description: ResourceError Class
//      Author: Ziming Li
//     Created: 2020-01-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2013 Google Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_RESOURCE_ERROR_H
#define BLINKIT_BLINK_RESOURCE_ERROR_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"
#include "url/bk_url.h"

namespace blink {

// ResourceError represents an error for loading a resource. There is no
// "no-error" instance. Use std::optional for nullable errors.
class ResourceError final
{
    DISALLOW_NEW();
public:
    ResourceError(int errorCode, const BlinKit::BkURL &failingURL);

    int ErrorCode(void) const { return m_errorCode; }
    std::string FailingURL(void) const { return m_failingURL.AsString(); }

    bool IsCancellation(void) const;

    static bool Compare(const ResourceError &a, const ResourceError &b);
private:
    int m_errorCode;
    BlinKit::BkURL m_failingURL;
};

inline bool operator==(const ResourceError &a, const ResourceError &b)
{
    return ResourceError::Compare(a, b);
}
inline bool operator!=(const ResourceError &a, const ResourceError &b)
{
    return !(a == b);
}

} // namespace blink

#endif // BLINKIT_BLINK_RESOURCE_ERROR_H
