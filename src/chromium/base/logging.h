// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: logging.h
// Description: Placeholders for Logging
//      Author: Ziming Li
//     Created: 2019-02-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_LOGGING_H
#define BLINKIT_BASE_LOGGING_H

#pragma once

#include <cassert>
#include "build/build_config.h"
#include "base/compiler_specific.h"
#include "base/macros.h"

#ifdef OS_WIN
#   ifdef ASSERT
#       undef ASSERT
#   endif
#   ifdef NDEBUG
#       define ASSERT(...)  ((void)0)
#   else
#       define ASSERT(e)    (!(e) ? __debugbreak() : (void)0)
#   endif
#else
#   define ASSERT   assert
#endif

namespace BlinKit {

class Asserter {
public:
    explicit Asserter(bool condition) { ASSERT(condition); }
};

void BkLog(const char *format, ...);

} // namespace BlinKit

template <typename T>
inline BlinKit::Asserter& operator<<(const BlinKit::Asserter &a, const T &) {
    return const_cast<BlinKit::Asserter &>(a);
}

#ifdef NDEBUG
#   define DCHECK_IS_ON()   0
#   define BKLOG(...)       ((void)0)
#else
#   define DCHECK_IS_ON()   1
#   define BKLOG            BlinKit::BkLog
#endif

#define BKASSERT(condition) (::BlinKit::Asserter(!!(condition)))

#define CHECK    BKASSERT
#define DCHECK   BKASSERT

#define CHECK_EQ(v1, v2)    BKASSERT((v1) == (v2))
#define CHECK_GE(v1, v2)    BKASSERT((v1) >= (v2))
#define CHECK_GT(v1, v2)    BKASSERT((v1) > (v2))
#define CHECK_LE(v1, v2)    BKASSERT((v1) <= (v2))
#define CHECK_LT(v1, v2)    BKASSERT((v1) < (v2))
#define CHECK_NE(v1, v2)    BKASSERT((v1) != (v2))

#define DCHECK_EQ(v1, v2)   BKASSERT((v1) == (v2))
#define DCHECK_GE(v1, v2)   BKASSERT((v1) >= (v2))
#define DCHECK_GT(v1, v2)   BKASSERT((v1) > (v2))
#define DCHECK_LE(v1, v2)   BKASSERT((v1) <= (v2))
#define DCHECK_LT(v1, v2)   BKASSERT((v1) < (v2))
#define DCHECK_NE(v1, v2)   BKASSERT((v1) != (v2))

#define DLOG(x)             BKASSERT(false)
#define DVLOG(x)            BKASSERT(false)
#define NOTIMPLEMENTED()    BKASSERT(false)
#define NOTREACHED()        BKASSERT(false)
#define VLOG(x)             BKASSERT(false)

#endif // BLINKIT_BASE_LOGGING_H
