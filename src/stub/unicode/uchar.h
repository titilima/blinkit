// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: uchar.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-02-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_UCHAR_H
#define BLINKIT_STUB_UCHAR_H

#pragma once

#include <cstdint>

#if defined(OS_WIN)
typedef wchar_t UChar;
#endif

typedef int32_t UChar32;

#define UCHAR_MAX_VALUE 0x10ffff

#define U_IS_BMP(c)         ((uint32_t)(c)<=0xffff)
#define U_IS_SURROGATE(c)   (((c)&0xfffff800)==0xd800)
#define U_MASK(x)           ((uint32_t)1<<(x))

#define U16_SURROGATE_OFFSET                ((0xd800<<10UL)+0xdc00-0x10000)
#define U16_GET_SUPPLEMENTARY(lead, trail)  (((UChar32)(lead)<<10UL)+(UChar32)(trail)-U16_SURROGATE_OFFSET)

#define U16_IS_LEAD(c)              (((c)&0xfffffc00)==0xd800)
#define U16_IS_SURROGATE(c)         U_IS_SURROGATE(c)
#define U16_IS_SURROGATE_LEAD(c)    (((c)&0x400)==0)
#define U16_IS_TRAIL(c)             (((c)&0xfffffc00)==0xdc00)

#define U16_LEAD(supplementary)     (UChar)(((supplementary)>>10)+0xd7c0)
#define U16_TRAIL(supplementary)    (UChar)(((supplementary)&0x3ff)|0xdc00)

#define U16_NEXT(s, i, length, c) { \
    (c)=(s)[(i)++]; \
    if(U16_IS_LEAD(c)) { \
        uint16_t __c2; \
        if((i)!=(length) && U16_IS_TRAIL(__c2=(s)[(i)])) { \
            ++(i); \
            (c)=U16_GET_SUPPLEMENTARY((c), __c2); \
        } \
    } \
}

typedef enum UCharCategory
{
    /*
     * Note: UCharCategory constants and their API comments are parsed by preparseucd.py.
     * It matches pairs of lines like
     *     / ** <Unicode 2-letter General_Category value> comment... * /
     *     U_<[A-Z_]+> = <integer>,
     */

    /** Non-category for unassigned and non-character code points. @stable ICU 2.0 */
    U_UNASSIGNED              = 0,
    /** Cn "Other, Not Assigned (no characters in [UnicodeData.txt] have this property)" (same as U_UNASSIGNED!) @stable ICU 2.0 */
    U_GENERAL_OTHER_TYPES     = 0,
    /** Lu @stable ICU 2.0 */
    U_UPPERCASE_LETTER        = 1,
    /** Ll @stable ICU 2.0 */
    U_LOWERCASE_LETTER        = 2,
    /** Lt @stable ICU 2.0 */
    U_TITLECASE_LETTER        = 3,
    /** Lm @stable ICU 2.0 */
    U_MODIFIER_LETTER         = 4,
    /** Lo @stable ICU 2.0 */
    U_OTHER_LETTER            = 5,
    /** Mn @stable ICU 2.0 */
    U_NON_SPACING_MARK        = 6,
    /** Me @stable ICU 2.0 */
    U_ENCLOSING_MARK          = 7,
    /** Mc @stable ICU 2.0 */
    U_COMBINING_SPACING_MARK  = 8,
    /** Nd @stable ICU 2.0 */
    U_DECIMAL_DIGIT_NUMBER    = 9,
    /** Nl @stable ICU 2.0 */
    U_LETTER_NUMBER           = 10,
    /** No @stable ICU 2.0 */
    U_OTHER_NUMBER            = 11,
    /** Zs @stable ICU 2.0 */
    U_SPACE_SEPARATOR         = 12,
    /** Zl @stable ICU 2.0 */
    U_LINE_SEPARATOR          = 13,
    /** Zp @stable ICU 2.0 */
    U_PARAGRAPH_SEPARATOR     = 14,
    /** Cc @stable ICU 2.0 */
    U_CONTROL_CHAR            = 15,
    /** Cf @stable ICU 2.0 */
    U_FORMAT_CHAR             = 16,
    /** Co @stable ICU 2.0 */
    U_PRIVATE_USE_CHAR        = 17,
    /** Cs @stable ICU 2.0 */
    U_SURROGATE               = 18,
    /** Pd @stable ICU 2.0 */
    U_DASH_PUNCTUATION        = 19,
    /** Ps @stable ICU 2.0 */
    U_START_PUNCTUATION       = 20,
    /** Pe @stable ICU 2.0 */
    U_END_PUNCTUATION         = 21,
    /** Pc @stable ICU 2.0 */
    U_CONNECTOR_PUNCTUATION   = 22,
    /** Po @stable ICU 2.0 */
    U_OTHER_PUNCTUATION       = 23,
    /** Sm @stable ICU 2.0 */
    U_MATH_SYMBOL             = 24,
    /** Sc @stable ICU 2.0 */
    U_CURRENCY_SYMBOL         = 25,
    /** Sk @stable ICU 2.0 */
    U_MODIFIER_SYMBOL         = 26,
    /** So @stable ICU 2.0 */
    U_OTHER_SYMBOL            = 27,
    /** Pi @stable ICU 2.0 */
    U_INITIAL_PUNCTUATION     = 28,
    /** Pf @stable ICU 2.0 */
    U_FINAL_PUNCTUATION       = 29,
    /** One higher than the last enum UCharCategory constant. @stable ICU 2.0 */
    U_CHAR_CATEGORY_COUNT
} UCharCategory;

typedef enum UDecompositionType {
    /*
     * Note: UDecompositionType constants are parsed by preparseucd.py.
     * It matches lines like
     *     U_DT_<Unicode Decomposition_Type value name>
     */

    U_DT_NONE,              /*[none]*/
    U_DT_CANONICAL,         /*[can]*/
    U_DT_COMPAT,            /*[com]*/
    U_DT_CIRCLE,            /*[enc]*/
    U_DT_FINAL,             /*[fin]*/
    U_DT_FONT,              /*[font]*/
    U_DT_FRACTION,          /*[fra]*/
    U_DT_INITIAL,           /*[init]*/
    U_DT_ISOLATED,          /*[iso]*/
    U_DT_MEDIAL,            /*[med]*/
    U_DT_NARROW,            /*[nar]*/
    U_DT_NOBREAK,           /*[nb]*/
    U_DT_SMALL,             /*[sml]*/
    U_DT_SQUARE,            /*[sqr]*/
    U_DT_SUB,               /*[sub]*/
    U_DT_SUPER,             /*[sup]*/
    U_DT_VERTICAL,          /*[vert]*/
    U_DT_WIDE,              /*[wide]*/
    U_DT_COUNT /* 18 */
} UDecompositionType;

#endif // BLINKIT_STUB_UCHAR_H
