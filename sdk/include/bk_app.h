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

BKEXPORT bool_t BKAPI BkInitialize(void *reserved);
BKEXPORT void BKAPI BkFinalize(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_APP_H
