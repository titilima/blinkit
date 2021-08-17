// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: icu_stub.cpp
// Description: ICU Implementations
//      Author: Ziming Li
//     Created: 2021-08-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include <unicode/uchar.h>

UChar32 u_foldCase(UChar32 c, uint32_t options)
{
    ASSERT(U_FOLD_CASE_DEFAULT == options);

    wchar_t wch = static_cast<wchar_t>(c);
    LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, &wch, 1, &wch, 1);
    return wch;
}
