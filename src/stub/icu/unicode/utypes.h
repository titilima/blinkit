// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: utypes.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-02-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UTYPES_H
#define BLINKIT_STUB_ICU_UTYPES_H

#pragma once

#include "utf.h"

typedef enum UErrorCode {
    U_ZERO_ERROR = 0,
    U_BUFFER_OVERFLOW_ERROR = 15,
} UErrorCode;

inline UBool U_SUCCESS(UErrorCode code) {
    return (UBool)(code <= U_ZERO_ERROR);
}

#endif // BLINKIT_STUB_ICU_UTYPES_H
