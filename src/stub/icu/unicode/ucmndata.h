// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: ucmndata.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UCMNDATA_H
#define BLINKIT_STUB_ICU_UCMNDATA_H

#pragma once

#include "udata.h"

typedef struct {
    uint16_t    headerSize;
    uint8_t     magic1;
    uint8_t     magic2;
} MappedData;

typedef struct {
    MappedData  dataHeader;
    UDataInfo   info;
} DataHeader;

typedef const DataHeader* (*LookupFn)(const UDataMemory *, const char *, int32_t *, UErrorCode *);

typedef uint32_t (*NumEntriesFn)(const UDataMemory *);

typedef struct {
    LookupFn      Lookup;
    NumEntriesFn  NumEntries;
} commonDataFuncs;

#endif // BLINKIT_STUB_ICU_UCMNDATA_H
