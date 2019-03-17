// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: ubidi_props.cpp
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "unicode/uchar.h"
#include "unicode/udatamem.h"
#include "ubidi_props.h"
#include "utrie2.h"

struct UBiDiProps {
    UDataMemory *mem;
    const int32_t *indexes;
    const uint32_t *mirrors;
    const uint8_t *jgArray;
    const uint8_t *jgArray2;

    UTrie2 trie;
    uint8_t formatVersion[4];
};

#define INCLUDED_FROM_UBIDI_PROPS_C
#include "ubidi_props_data.h"

UCharDirection ubidi_getClass(const UBiDiProps *bdp, UChar32 c)
{
    uint16_t props = UTRIE2_GET16(&bdp->trie, c);
    return (UCharDirection)UBIDI_GET_CLASS(props);
}

UCharDirection u_charDirection(UChar32 c)
{
    return ubidi_getClass(&ubidi_props_singleton, c);
}
