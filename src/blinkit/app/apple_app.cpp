// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_app.cpp
// Description: AppleApp Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_app.h"

namespace BlinKit {

void AppImpl::CreateInstance(void)
{
    new AppleApp;
}

void AppImpl::Log(const char *s)
{
    NSLog(@"%s", s);
}

} // namespace BlinKit
