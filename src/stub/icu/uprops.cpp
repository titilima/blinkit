// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: uprops.cpp
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-03
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "unicode/uchar.h"

#include <cassert>

int32_t u_getIntPropertyValue(UChar32 c, UProperty which)
{
    if (which < UCHAR_INT_START)
    {
        if (UCHAR_BINARY_START <= which && which < UCHAR_BINARY_LIMIT)
        {
            assert(false); // BKTODO:
            return 0;
#if 0
            const BinaryProperty &prop = binProps[which];
            return prop.contains(prop, c, which);
#endif
        }
    }
    else if (which < UCHAR_INT_LIMIT)
    {
        assert(false); // BKTODO:
        return 0;
#if 0
        const IntProperty &prop = intProps[which - UCHAR_INT_START];
        return prop.getValue(prop, c, which);
#endif
    }
    else if (which == UCHAR_GENERAL_CATEGORY_MASK)
    {
        return U_MASK(u_charType(c));
    }
    return 0;  // undefined
}
