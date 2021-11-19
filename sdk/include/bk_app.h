// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_app.h
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_APP_H
#define BLINKIT_SDK_APP_H

#pragma once

#include "bk_def.h"
#ifdef _WIN32
#   include <WinUser.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct BkAppClient {
    size_t SizeOfStruct; // sizeof(BkAppClient)
    void *UserData;
    void (BKAPI * Exit)(void *);
    bool_t (BKAPI * LoadResource)(const char *, struct BkBuffer *, void *);
};

BKEXPORT bool_t BKAPI BkInitialize(struct BkAppClient *client);

#ifdef _WIN32

BKEXPORT int BKAPI BkRunMessageLoop(void);

typedef bool_t (BKAPI * BkMessageFilter)(const MSG *, void *);
BKEXPORT int BKAPI BkRunMessageLoopEx(BkMessageFilter filter, void *userData);

#endif

BKEXPORT bool_t BKAPI IsBlinKitThread(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_APP_H
