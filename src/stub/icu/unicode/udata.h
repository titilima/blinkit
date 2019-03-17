// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: udata.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UDATA_H
#define BLINKIT_STUB_ICU_UDATA_H

#pragma once

#include "utypes.h"

typedef struct {
    uint16_t size;
    uint16_t reservedWord;
    uint8_t isBigEndian;
    uint8_t charsetFamily;
    uint8_t sizeofUChar;
    uint8_t reservedByte;
    uint8_t dataFormat[4];
    uint8_t formatVersion[4];
    uint8_t dataVersion[4];
} UDataInfo;

typedef struct UDataMemory UDataMemory;

#endif // BLINKIT_STUB_ICU_UDATA_H
