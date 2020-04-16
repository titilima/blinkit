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

enum BkAppMode {
    BK_APP_FOREGROUND_MODE = 0,
    BK_APP_BACKGROUND_MODE
};

BKEXPORT bool_t BKAPI BkInitialize(int mode, void *reserved);

/**
 * If you have your own message loops, call BkFinalize before application exiting.
 *   Cannot be used with BkRunApp.
 *   Foreground mode only.
 */
BKEXPORT void BKAPI BkFinalize(void);

/**
 * BkRunApp starts a message loop, and BlinKit will do the cleaning up stuff automatically.
 *   Cannot be used with BkFinalize.
 *   Foreground mode only.
 */
BKEXPORT int BKAPI BkRunApp(void);

/**
 * BkExitApp exits the message loop which created by BkRunApp or in backgound mode.
 */
BKEXPORT void BKAPI BkExitApp(int code);

/**
 * Execute code in the backgound thread.
 */
typedef void (BKAPI * BkBackgroundWorker)(void *);
BKEXPORT bool_t BKAPI BkAppExecute(BkBackgroundWorker worker, void *userData);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_APP_H
