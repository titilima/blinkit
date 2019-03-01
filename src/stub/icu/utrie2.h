// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: utrie2.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-03-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UTRIE2_H
#define BLINKIT_STUB_ICU_UTRIE2_H

#pragma once

#define UTRIE2_GET16(trie, c) _UTRIE2_GET((trie), index, (trie)->indexLength, (c))

struct UNewTrie2;

struct UTrie2 {
    const uint16_t *index;
    const uint16_t *data16;
    const uint32_t *data32;

    int32_t indexLength, dataLength;
    uint16_t index2NullOffset;
    uint16_t dataNullOffset;
    uint32_t initialValue;
    uint32_t errorValue;

    UChar32 highStart;
    int32_t highValueIndex;

    void *memory;
    int32_t length;
    UBool isMemoryOwned;
    UBool padding1;
    int16_t padding2;
    UNewTrie2 *newTrie;
};

enum {
    UTRIE2_SHIFT_1 = 6 + 5,
    UTRIE2_SHIFT_2 = 5,
    UTRIE2_SHIFT_1_2 = UTRIE2_SHIFT_1 - UTRIE2_SHIFT_2,
    UTRIE2_OMITTED_BMP_INDEX_1_LENGTH = 0x10000 >> UTRIE2_SHIFT_1,
    UTRIE2_INDEX_2_BLOCK_LENGTH = 1 << UTRIE2_SHIFT_1_2,
    UTRIE2_INDEX_2_MASK = UTRIE2_INDEX_2_BLOCK_LENGTH - 1,
    UTRIE2_DATA_BLOCK_LENGTH = 1 << UTRIE2_SHIFT_2,
    UTRIE2_DATA_MASK = UTRIE2_DATA_BLOCK_LENGTH - 1,
    UTRIE2_INDEX_SHIFT = 2,
    UTRIE2_LSCP_INDEX_2_OFFSET = 0x10000 >> UTRIE2_SHIFT_2,
    UTRIE2_LSCP_INDEX_2_LENGTH = 0x400 >> UTRIE2_SHIFT_2,
    UTRIE2_INDEX_2_BMP_LENGTH = UTRIE2_LSCP_INDEX_2_OFFSET + UTRIE2_LSCP_INDEX_2_LENGTH,
    UTRIE2_UTF8_2B_INDEX_2_OFFSET = UTRIE2_INDEX_2_BMP_LENGTH,
    UTRIE2_UTF8_2B_INDEX_2_LENGTH = 0x800 >> 6,
    UTRIE2_INDEX_1_OFFSET = UTRIE2_UTF8_2B_INDEX_2_OFFSET + UTRIE2_UTF8_2B_INDEX_2_LENGTH,
    UTRIE2_BAD_UTF8_DATA_OFFSET = 0x80,
};

#define _UTRIE2_INDEX_RAW(offset, trieIndex, c) \
    (((int32_t)((trieIndex)[(offset)+((c)>>UTRIE2_SHIFT_2)]) \
    <<UTRIE2_INDEX_SHIFT)+ \
    ((c)&UTRIE2_DATA_MASK))

#define _UTRIE2_INDEX_FROM_SUPP(trieIndex, c) \
    (((int32_t)((trieIndex)[ \
        (trieIndex)[(UTRIE2_INDEX_1_OFFSET-UTRIE2_OMITTED_BMP_INDEX_1_LENGTH)+ \
                      ((c)>>UTRIE2_SHIFT_1)]+ \
        (((c)>>UTRIE2_SHIFT_2)&UTRIE2_INDEX_2_MASK)]) \
    <<UTRIE2_INDEX_SHIFT)+ \
    ((c)&UTRIE2_DATA_MASK))

#define _UTRIE2_INDEX_FROM_CP(trie, asciiOffset, c) \
    ((uint32_t)(c)<0xd800 ? \
        _UTRIE2_INDEX_RAW(0, (trie)->index, c) : \
        (uint32_t)(c)<=0xffff ? \
            _UTRIE2_INDEX_RAW( \
                (c)<=0xdbff ? UTRIE2_LSCP_INDEX_2_OFFSET-(0xd800>>UTRIE2_SHIFT_2) : 0, \
                (trie)->index, c) : \
            (uint32_t)(c)>0x10ffff ? \
                (asciiOffset)+UTRIE2_BAD_UTF8_DATA_OFFSET : \
                (c)>=(trie)->highStart ? \
                    (trie)->highValueIndex : \
                    _UTRIE2_INDEX_FROM_SUPP((trie)->index, c))

#define _UTRIE2_GET(trie, data, asciiOffset, c) \
    (trie)->data[_UTRIE2_INDEX_FROM_CP(trie, asciiOffset, c)]

#endif // BLINKIT_STUB_ICU_UTRIE2_H
