// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: login_task.h
// Description: LoginTask Class
//      Author: Ziming Li
//     Created: 2020-04-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LOGIN_TASK_H
#define BLINKIT_BLINKIT_LOGIN_TASK_H

#pragma once

class LoginProxyImpl;

namespace BlinKit {

class LoginTask
{
public:
    virtual ~LoginTask(void) = default;

    virtual LoginTask* Execute(LoginProxyImpl &loginProxy) = 0;
protected:
    LoginTask(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOGIN_TASK_H
