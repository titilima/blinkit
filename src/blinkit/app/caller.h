// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: caller.h
// Description: Caller Classes
//      Author: Ziming Li
//     Created: 2021-01-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CALLER_H
#define BLINKIT_BLINKIT_CALLER_H

#pragma once

#include <functional>
#include "base/location.h"

namespace BlinKit {

class AppCaller
{
public:
    virtual ~AppCaller(void) = default;

    virtual void Call(const base::Location &loc, std::function<void()> &&task) = 0;
    virtual void SyncCall(const base::Location &loc, std::function<void()> &&task) = 0;
protected:
    AppCaller(void) = default;
};

class ClientCaller
{
public:
    virtual ~ClientCaller(void) = default;

    virtual void Post(const base::Location &loc, std::function<void()> &&task) = 0;
#ifndef NDEBUG
    virtual bool IsClientThread(void) const = 0;
#endif
protected:
    ClientCaller(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CALLER_H
