// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cf.h
// Description: CoreFoundation Wrappers
//      Author: Ziming Li
//     Created: 2020-04-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CF_H
#define BLINKIT_BLINKIT_CF_H

#pragma once

#include <string>
#include "base/mac/scoped_cftyperef.h"

namespace BlinKit {
namespace CF {

class StaticString final : public base::ScopedCFTypeRef<CFStringRef>
{
public:
    StaticString(const char *ps);
    StaticString(const std::string &s);
};

class StaticURL final : public base::ScopedCFTypeRef<CFURLRef>
{
public:
    StaticURL(const StaticString &s);
};

} // namespace CF
} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CF_H
