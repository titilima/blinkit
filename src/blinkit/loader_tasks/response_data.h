// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_data.h
// Description: ResponseData Structure
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RESPONSE_DATA_H
#define BLINKIT_BLINKIT_RESPONSE_DATA_H

#pragma once

#include "platform/weborigin/KURL.h"

namespace BlinKit {

struct ResponseData final : public std::enable_shared_from_this<ResponseData>
{
    blink::KURL URI;
    int StatusCode = 200;
    std::string ContentType;
    WTF::AtomicString MIMEType;
    WTF::String TextEncoding;
    std::vector<unsigned char> Body;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RESPONSE_DATA_H
