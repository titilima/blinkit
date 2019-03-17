// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: udatamem.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UDATAMEM_H
#define BLINKIT_STUB_ICU_UDATAMEM_H

#pragma once

#include "ucmndata.h"

struct UDataMemory {
    const commonDataFuncs *vFuncs;
    const DataHeader *pHeader;
    const void *toc;
    UBool heapAllocated;
    void *mapAddr;
    void *map;
    int32_t length;
};

#endif // BLINKIT_STUB_ICU_UDATAMEM_H
