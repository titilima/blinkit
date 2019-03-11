// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: BlinKit.h
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_BLINKIT_H
#define BLINKIT_SDK_BLINKIT_H

#pragma once

#ifdef _WIN32
#   define BKAPI    __stdcall
#   ifndef BLINKIT_EXPORTS
#       define BKEXPORT __declspec(dllimport)
#   endif
#endif

#ifndef BKAPI
#   define BKAPI
#endif

#ifndef BKEXPORT
#   define BKEXPORT
#endif

namespace BlinKit {

class BkAppClient {
};

class BkApp {
public:
    virtual void BKAPI Exit(void) = 0;
};

} // namespace BlinKit

extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkInitialize(BlinKit::BkAppClient *client);
extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkGetApp(void);

#endif // BLINKIT_SDK_BLINKIT_H
