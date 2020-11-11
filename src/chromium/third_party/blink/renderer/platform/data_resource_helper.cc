// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: data_resource_helper.cc
// Description: GetDataResourceAsASCIIString
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "data_resource_helper.h"

#include "third_party/blink/public/platform/platform.h"

namespace blink {

String GetDataResourceAsASCIIString(const char *resource)
{
    std::string resourceData = Platform::Current()->GetDataResource(resource);

    String ret = String::FromStdUTF8(resourceData);
    ASSERT(ret.ContainsOnlyASCII());
    return ret;
}

} // namespace blink
