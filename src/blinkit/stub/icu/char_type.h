// -------------------------------------------------
// BlinKit - BkCrawler Library
// -------------------------------------------------
//   File Name: char_type.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2020-12-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCRAWLER_CHAR_TYPE_H
#define BLINKIT_BKCRAWLER_CHAR_TYPE_H

#pragma once

#ifndef BLINKIT_CRAWLER_ONLY
#   error This file is for BkCrawler only!
#endif

#include <unicode/uchar.h>
#include "base/strings/string_util.h"

namespace BlinKit {

// CharCategory::kNumber_DecimalDigit
bool IsDecimalDigit(UChar32 c);

// CharCategory::kMark_Enclosing
bool IsEnclosingMark(UChar32 c);

// CharCategory::kLetter_Modifier
bool IsModifierLetter(UChar32 c);

// CharCategory::kMark_NonSpacing
bool IsNonSpacingMark(UChar32 c);

// CharCategory::kNumber_Letter
bool IsNumberLetter(UChar32 c);

// CharCategory::kLetter_Other
bool IsOtherLetter(UChar32 c);

// CharCategory::kMark_SpacingCombining
bool IsSpacingCombiningMark(UChar32 c);

// CharCategory::kLetter_Titlecase
bool IsTitlecaseLetter(UChar32 c);

} // namespace BlinKit

#endif // BLINKIT_BKCRAWLER_CHAR_TYPE_H
