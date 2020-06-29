// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_task.h
// Description: ProxyTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_PROXY_TASK_H
#define BLINKIT_BKHELPER_PROXY_TASK_H

#pragma once

class ProxyImpl;

namespace BlinKit {

class ProxyTask
{
public:
    virtual ~ProxyTask(void) = default;

    virtual ProxyTask* Execute(ProxyImpl &proxy) = 0;
protected:
    ProxyTask(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_PROXY_TASK_H
