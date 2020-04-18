// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ns.mm
// Description: Foundation Wrappers
//      Author: Ziming Li
//     Created: 2020-04-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "ns.h"

namespace BlinKit {
namespace NS {

NSString* StringFromStd(const std::string &s)
{
    return [[NSString alloc] initWithBytes: s.data()
                                    length: s.length()
                                  encoding: NSUTF8StringEncoding];
}

std::string StringToStd(NSString *s)
{
    NSUInteger l = [s lengthOfBytesUsingEncoding: NSUTF8StringEncoding];
    std::string ret(l, '\0');
    [s getCString: const_cast<char *>(ret.c_str()) maxLength: l + 1 encoding: NSUTF8StringEncoding];
    return ret;
}

NSURL* StringToURL(const std::string &s)
{
    NSString *urlString = StringFromStd(s);
    return [NSURL URLWithString: urlString];
}

} // namespace NS
} // namespace BlinKit
