// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: rand_util_generic.cpp
// Description: Rand Implementations
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "base/rand_util.h"

#include <mutex>

namespace base {

void RandBytes(void* output, size_t output_length)
{
    static std::once_flag s_flag;
    std::call_once(s_flag, [] { srand(time(nullptr)); });

    unsigned char *dst = reinterpret_cast<unsigned char *>(output);
    for (size_t i = 0; i < output_length; ++i)
    {
        int n = rand() % 256;
        *dst = static_cast<unsigned char>(n);
        ++dst;
    }
}

} // namespace base
