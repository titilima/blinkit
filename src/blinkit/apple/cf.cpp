// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cf.cpp
// Description: CoreFoundation Wrappers
//      Author: Ziming Li
//     Created: 2020-04-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "cf.h"

namespace BlinKit {
namespace CF {

StaticString::StaticString(const char *ps)
    : base::ScopedCFTypeRef<CFStringRef>(CFStringCreateWithCStringNoCopy(nullptr, ps, kCFStringEncodingUTF8, kCFAllocatorNull))
{
}

StaticString::StaticString(const std::string &s)
    : base::ScopedCFTypeRef<CFStringRef>(CFStringCreateWithBytesNoCopy(nullptr, reinterpret_cast<const UInt8 *>(s.data()), s.length(), kCFStringEncodingUTF8, false, kCFAllocatorNull))
{
}

StaticURL::StaticURL(const StaticString &s) : base::ScopedCFTypeRef<CFURLRef>(CFURLCreateWithString(nullptr, s, nullptr))
{
}

} // namespace CF
} // namespace BlinKit
