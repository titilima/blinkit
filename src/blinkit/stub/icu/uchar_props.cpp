// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: uchar_props.cpp
// Description: ICU Implementations
//      Author: Ziming Li
//     Created: 2021-09-07
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include <unicode/uchar.h>

#include "chromium/third_party/icu/source/common/uprops.h"
#include "chromium/third_party/icu/source/common/utrie2.h"

#define INCLUDED_FROM_UCHAR_C
#include "chromium/third_party/icu/source/common/uchar_props_data.h"

#define GET_PROPS(c, result) ((result)=UTRIE2_GET16(&propsTrie, c));

int8_t u_charType(UChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (int8_t)GET_CATEGORY(props);
}
