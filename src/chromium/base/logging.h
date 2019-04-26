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

namespace BlinKit {

class Asserter {
public:
    explicit Asserter(bool condition) { assert(condition); }
};

void BkLog(const char *format, ...);

} // namespace BlinKit

template <typename T>
inline BlinKit::Asserter& operator<<(const BlinKit::Asserter &a, const T &) {
    return const_cast<BlinKit::Asserter &>(a);
}

#ifdef _DEBUG
#   define DCHECK_IS_ON()   1
#   define BKLOG            BlinKit::BkLog
#else
#   define DCHECK_IS_ON()   0
#   define BKLOG(...)
#endif

#define BKASSERT(condition) ::BlinKit::Asserter(!!(condition))

#define CHECK       BKASSERT
#define DCHECK      BKASSERT

#define DCHECK_EQ(v1, v2)   BKASSERT((v1) == (v2))
#define DCHECK_GE(v1, v2)   BKASSERT((v1) >= (v2))
#define DCHECK_GT(v1, v2)   BKASSERT((v1) > (v2))
#define DCHECK_LE(v1, v2)   BKASSERT((v1) <= (v2))
#define DCHECK_LT(v1, v2)   BKASSERT((v1) < (v2))
#define DCHECK_NE(v1, v2)   BKASSERT((v1) != (v2))

#define DVLOG(x)        BKASSERT(false)
#define NOTREACHED()    BKASSERT(false)
#define VLOG(x)        BKASSERT(false)

#endif // BLINKIT_BASE_LOGGING_H
