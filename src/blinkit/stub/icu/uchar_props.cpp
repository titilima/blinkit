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

#include "chromium/third_party/icu/source/common/unicode/unorm2.h"
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IntProperty;

using IntPropertyGetValue = int32_t (*)(const IntProperty &, UChar32, UProperty);
using IntPropertyGetMaxValue = int32_t (*)(const IntProperty &, UProperty);

struct IntProperty {
    int32_t column;  // SRC_PROPSVEC column, or "source" if mask==0
    uint32_t mask;
    int32_t shift;  // =maxValue if getMaxValueFromShift() is used
    IntPropertyGetValue getValue;
    IntPropertyGetMaxValue getMaxValue;
};

static int32_t defaultGetValue(const IntProperty &prop, UChar32 c, UProperty)
{
    // systematic, directly stored properties
    return static_cast<int32_t>(u_getUnicodeProperties(c, prop.column) & prop.mask) >> prop.shift;
}

static int32_t getBiDiClass(const IntProperty &, UChar32 c, UProperty)
{
    return static_cast<int32_t>(u_charDirection(c));
}

static constexpr IntPropertyGetValue getBiDiPairedBracketType = nullptr;
static constexpr IntPropertyGetValue getCombiningClass = nullptr;

static int32_t getGeneralCategory(const IntProperty &, UChar32 c, UProperty)
{
    return static_cast<int32_t>(u_charType(c));
}

static constexpr IntPropertyGetValue getJoiningGroup = nullptr;
static constexpr IntPropertyGetValue getJoiningType = nullptr;
static constexpr IntPropertyGetValue getNumericType = nullptr;
static constexpr IntPropertyGetValue getScript = nullptr;
static constexpr IntPropertyGetValue getHangulSyllableType = nullptr;
static constexpr IntPropertyGetValue getNormQuickCheck = nullptr;
static constexpr IntPropertyGetValue getLeadCombiningClass = nullptr;
static constexpr IntPropertyGetValue getTrailCombiningClass = nullptr;

static constexpr IntPropertyGetMaxValue biDiGetMaxValue = nullptr;
static constexpr IntPropertyGetMaxValue defaultGetMaxValue = nullptr;
static constexpr IntPropertyGetMaxValue getMaxValueFromShift = nullptr;

static const IntProperty intProps[UCHAR_INT_LIMIT - UCHAR_INT_START] = {
    /*
     * column, mask and shift values for int-value properties from u_getUnicodeProperties().
     * Must be in order of corresponding UProperty,
     * and there must be exactly one entry per int UProperty.
     *
     * Properties with mask==0 are handled in code.
     * For them, column is the UPropertySource value.
     */
    { UPROPS_SRC_BIDI,  0, 0,                               getBiDiClass, biDiGetMaxValue },
    { 0,                UPROPS_BLOCK_MASK, UPROPS_BLOCK_SHIFT, defaultGetValue, defaultGetMaxValue },
    { UPROPS_SRC_NFC,   0, 0xff,                            getCombiningClass, getMaxValueFromShift },
    { 2,                UPROPS_DT_MASK, 0,                  defaultGetValue, defaultGetMaxValue },
    { 0,                UPROPS_EA_MASK, UPROPS_EA_SHIFT,    defaultGetValue, defaultGetMaxValue },
    { UPROPS_SRC_CHAR,  0, (int32_t)U_CHAR_CATEGORY_COUNT - 1,getGeneralCategory, getMaxValueFromShift },
    { UPROPS_SRC_BIDI,  0, 0,                               getJoiningGroup, biDiGetMaxValue },
    { UPROPS_SRC_BIDI,  0, 0,                               getJoiningType, biDiGetMaxValue },
    { 2,                UPROPS_LB_MASK, UPROPS_LB_SHIFT,    defaultGetValue, defaultGetMaxValue },
    { UPROPS_SRC_CHAR,  0, (int32_t)U_NT_COUNT - 1,           getNumericType, getMaxValueFromShift },
    { 0,                UPROPS_SCRIPT_MASK, 0,              getScript, defaultGetMaxValue },
    { UPROPS_SRC_PROPSVEC, 0, (int32_t)U_HST_COUNT - 1,       getHangulSyllableType, getMaxValueFromShift },
    // UCHAR_NFD_QUICK_CHECK: max=1=YES -- never "maybe", only "no" or "yes"
    { UPROPS_SRC_NFC,   0, (int32_t)UNORM_YES,              getNormQuickCheck, getMaxValueFromShift },
    // UCHAR_NFKD_QUICK_CHECK: max=1=YES -- never "maybe", only "no" or "yes"
    { UPROPS_SRC_NFKC,  0, (int32_t)UNORM_YES,              getNormQuickCheck, getMaxValueFromShift },
    // UCHAR_NFC_QUICK_CHECK: max=2=MAYBE
    { UPROPS_SRC_NFC,   0, (int32_t)UNORM_MAYBE,            getNormQuickCheck, getMaxValueFromShift },
    // UCHAR_NFKC_QUICK_CHECK: max=2=MAYBE
    { UPROPS_SRC_NFKC,  0, (int32_t)UNORM_MAYBE,            getNormQuickCheck, getMaxValueFromShift },
    { UPROPS_SRC_NFC,   0, 0xff,                            getLeadCombiningClass, getMaxValueFromShift },
    { UPROPS_SRC_NFC,   0, 0xff,                            getTrailCombiningClass, getMaxValueFromShift },
    { 2,                UPROPS_GCB_MASK, UPROPS_GCB_SHIFT,  defaultGetValue, defaultGetMaxValue },
    { 2,                UPROPS_SB_MASK, UPROPS_SB_SHIFT,    defaultGetValue, defaultGetMaxValue },
    { 2,                UPROPS_WB_MASK, UPROPS_WB_SHIFT,    defaultGetValue, defaultGetMaxValue },
    { UPROPS_SRC_BIDI,  0, 0,                               getBiDiPairedBracketType, biDiGetMaxValue },
};

int32_t u_getIntPropertyValue(UChar32 c, UProperty which)
{
    if (which < UCHAR_INT_START)
    {
        if (UCHAR_BINARY_START <= which && which < UCHAR_BINARY_LIMIT)
        {
            ASSERT(false); // BKTODO:
#if 0
            const BinaryProperty &prop = binProps[which];
            return prop.contains(prop, c, which);
#endif
        }
    }
    else if (which < UCHAR_INT_LIMIT)
    {
        const IntProperty &prop = intProps[which - UCHAR_INT_START];
#if 0 // BKTODO:
        return prop.getValue(prop, c, which);
#else
        if (nullptr != prop.getValue)
            return prop.getValue(prop, c, which);
        else
            ASSERT(nullptr != prop.getValue);
#endif
    }
    else if (which == UCHAR_GENERAL_CATEGORY_MASK)
    {
        return U_MASK(u_charType(c));
    }
    return 0;  // undefined
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t u_getUnicodeProperties(UChar32 c, int32_t column)
{
    ASSERT(column >= 0);

    if (column >= propsVectorsColumns)
        return 0;

    uint16_t vecIndex = UTRIE2_GET16(&propsVectorsTrie, c);
    return propsVectors[vecIndex + column];
}
