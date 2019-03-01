// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: uchar.cpp
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "unicode/uchar.h"
#include "utrie2.h"
#include "uprops.h"

#define INCLUDED_FROM_UCHAR_C
#include "uchar_props_data.h"

#define GET_PROPS(c, result) ((result)=UTRIE2_GET16(&propsTrie, c))

int8_t u_charType(UChar32 c)
{
    uint32_t props;
    GET_PROPS(c, props);
    return (int8_t)GET_CATEGORY(props);
}
