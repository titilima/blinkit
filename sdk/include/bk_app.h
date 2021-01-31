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

#ifdef __cplusplus
extern "C" {
#endif

struct BkAppClient {
    size_t SizeOfStruct; // sizeof(BkAppClient)
    void *UserData;
    void (BKAPI * Exit)(void *);
};

BKEXPORT bool_t BKAPI BkInitialize(struct BkAppClient *client);

/**
 * If you have your own message loops, call BkFinalize before application exiting.
 *   Cannot be used with BkRunApp.
 *   Mainthread mode only.
 */
BKEXPORT void BKAPI BkFinalize(void);

BKEXPORT bool_t BKAPI IsBlinKitThread(void);

/**
 * Run crawler(s) in exclusive mode.
 */
BKEXPORT int BKAPI BkCrawlerMain(struct BkAppClient *client, void (BKAPI * Init)(void *));

/**
 * BkExitApp exits the message loop which created by BkRunApp or in backgound mode.
 */
BKEXPORT void BKAPI BkExit(int code);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_APP_H
