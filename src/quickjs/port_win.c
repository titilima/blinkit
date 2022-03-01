// -------------------------------------------------
// BlinKit - quickjs Library
// -------------------------------------------------
//   File Name: port_win.c
// Description: Ports for Windows
//      Author: Ziming Li
//     Created: 2022-03-01
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./port_win.h"

#include "third_party/zed/include/zed/assert.h"

void gettimeofday(struct timeval *dst, void *unused)
{
    SYSTEMTIME st = { 0 };

    ZASSERT(NULL == unused);

    dst->tv_sec = time(NULL);
    GetSystemTime(&st);
    dst->tv_usec = st.wMilliseconds * 1000;
}
