// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: controller_impl.h
// Description: ControllerImpl Class
//      Author: Ziming Li
//     Created: 2019-09-06
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CONTROLLER_IMPL_H
#define BLINKIT_BLINKIT_CONTROLLER_IMPL_H

#pragma once

#include "bk_def.h"

class ControllerImpl
{
public:
    virtual int Release(void) = 0;
    virtual int ContinueWorking(void) = 0;
    virtual int CancelWork(void) = 0;
};

#endif // BLINKIT_BLINKIT_CONTROLLER_IMPL_H
