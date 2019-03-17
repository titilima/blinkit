// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: ubidi_props.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UBIDI_PROPS_H
#define BLINKIT_STUB_ICU_UBIDI_PROPS_H

#pragma once

#include "unicode/utypes.h"

struct UBiDiProps;
typedef struct UBiDiProps UBiDiProps;

enum {
    UBIDI_IX_TOP = 16
};

#define UBIDI_CLASS_MASK        0x0000001f

#define UBIDI_GET_CLASS(props) ((props)&UBIDI_CLASS_MASK)

#endif // BLINKIT_STUB_ICU_UBIDI_PROPS_H
