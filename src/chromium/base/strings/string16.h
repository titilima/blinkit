// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: string16.h
// Description: string16 Definitions
//      Author: Ziming Li
//     Created: 2019-08-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_STRING16_H
#define BLINKIT_BASE_STRING16_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "base/base_export.h"
#include "build/build_config.h"
#if (OS_MACOSX || OS_IOS)
#   include <MacTypes.h>
#endif

namespace base {

#if defined(WCHAR_T_IS_UTF16)
typedef wchar_t char16;
#elif defined(WCHAR_T_IS_UTF32)
#   if (OS_MACOSX || OS_IOS)
typedef UniChar char16;
#   else
typedef char16_t char16;
#   endif
#endif
typedef std::basic_string<char16> string16;

}  // namespace base

#endif  // BLINKIT_BASE_STRING16_H
