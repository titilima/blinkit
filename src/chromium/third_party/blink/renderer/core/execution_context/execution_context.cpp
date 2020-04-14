// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: execution_context.cpp
// Description: ExecutionContext Class
//      Author: Ziming Li
//     Created: 2020-02-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "execution_context.h"

#include "blinkit/common/bk_url.h"

namespace blink {

// https://w3c.github.io/webappsec-referrer-policy/#determine-requests-referrer
String ExecutionContext::OutgoingReferrer(void) const
{
    // Step 3.1: "If environment's global object is a Window object, then"
    // This case is implemented in Document::OutgoingReferrer().

    // Step 3.2: "Otherwise, let referrerSource be environment's creation URL."
    return String::FromStdUTF8(Url().StrippedForUseAsReferrer());
}

} // namespace blink
