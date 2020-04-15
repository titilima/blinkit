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

#include "base/mac/scoped_cftyperef.h"

namespace BlinKit {

AppleApp::AppleApp(int mode) : AppImpl(mode)
{
}

AppleApp::~AppleApp(void) = default;

void AppleApp::Exit(int code)
{
    ASSERT(false); // BKTODO:
}

int AppleApp::RunAndFinalize(void)
{
    ASSERT(false); // BKTODO:
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AppImpl* AppImpl::CreateInstance(int mode)
{
    return new AppleApp(mode);
}

void AppImpl::InitializeBackgroundInstance(void)
{
    ASSERT(false); // BKTODO:
}

void AppImpl::Log(const char *s)
{
    base::ScopedCFTypeRef<CFStringRef> cf = CFStringCreateWithCStringNoCopy(nullptr, s, kCFStringEncodingUTF8, nullptr);
    CFShowStr(cf);
}

} // namespace BlinKit
