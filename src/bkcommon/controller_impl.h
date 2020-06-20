// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: controller_impl.h
// Description: ControllerImpl Class
//      Author: Ziming Li
//     Created: 2019-09-06
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_CONTROLLER_IMPL_H
#define BLINKIT_BKCOMMON_CONTROLLER_IMPL_H

#pragma once

class ControllerImpl
{
public:
    virtual int Release(void) = 0;
    virtual int ContinueWorking(void) = 0;
    virtual int CancelWork(void) = 0;
};

#endif // BLINKIT_BKCOMMON_CONTROLLER_IMPL_H
