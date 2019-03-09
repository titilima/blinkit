// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_impl.h
// Description: AppImpl Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APP_IMPL_H
#define BLINKIT_BLINKIT_APP_IMPL_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "public/platform/Platform.h"

namespace BlinKit {

class AppImpl : public BkApp, public blink::Platform
{
public:
    static void CreateInstance(void);
    virtual ~AppImpl(void);

    void Initialize(BkAppClient *client);
private:
    BkAppClient *m_client = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
