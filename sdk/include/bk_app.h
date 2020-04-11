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
    BK_APP_DEFAULT_MODE = 0,
    BK_APP_BACKGROUND_MODE
};

struct BkInitData {
    size_t size; // sizeof(BkInitData)
    int mode;
    void *reserved;
};

BKEXPORT bool_t BKAPI BkInitialize(struct BkInitData *initData);

BKEXPORT void BKAPI BkFinalize(void);

typedef void (BKAPI * BkBackgroundWorker)(void *);
BKEXPORT bool_t BKAPI BkAppExecute(BkBackgroundWorker worker, void *userData);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_APP_H
