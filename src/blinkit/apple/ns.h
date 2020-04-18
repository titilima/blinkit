// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ns.h
// Description: Foundation Wrappers
//      Author: Ziming Li
//     Created: 2020-04-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_NS_H
#define BLINKIT_BLINKIT_NS_H

#pragma once

#include <Foundation/Foundation.h>

namespace BlinKit {
namespace NS {

NSString* StringFromStd(const std::string &s);
std::string StringToStd(NSString *s);

NSURL* StringToURL(const std::string &s);

} // namespace NS
} // namespace BlinKit

#endif // BLINKIT_BLINKIT_NS_H
