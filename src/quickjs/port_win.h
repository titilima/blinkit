#pragma once
// -------------------------------------------------
// BlinKit - quickjs Library
// -------------------------------------------------
//   File Name: port_win.h
// Description: Ports for Windows
//      Author: Ziming Li
//     Created: 2022-03-01
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QUICKJS_PORT_WIN_H
#define BLINKIT_QUICKJS_PORT_WIN_H

#include <time.h>

struct timeval {
    time_t tv_sec;
    int tv_usec;
};

void gettimeofday(struct timeval *dst, void *);

#endif // BLINKIT_QUICKJS_PORT_WIN_H
