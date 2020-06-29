// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: last_response_task.h
// Description: LastResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_LAST_RESPONSE_TASK_H
#define BLINKIT_BKHELPER_LAST_RESPONSE_TASK_H

#pragma once

#include "bkhelper/proxy/socket_wrapper.h"
#include "bkhelper/proxy/tasks/proxy_task.h"

namespace BlinKit {

class LastResponseTask final : public ProxyTask
{
public:
    LastResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper);
private:
    ProxyTask* Execute(ProxyImpl &proxy) override;

    std::shared_ptr<SocketWrapper> m_socketWrapper;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_LAST_RESPONSE_TASK_H
