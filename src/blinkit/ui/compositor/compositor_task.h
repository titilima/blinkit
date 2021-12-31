#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor_task.h
// Description: CompositorTask Class
//      Author: Ziming Li
//     Created: 2021-12-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMPOSITOR_TASK_H
#define BLINKIT_COMPOSITOR_TASK_H

namespace BlinKit {

class Compositor;

class CompositorTask
{
public:
    virtual ~CompositorTask(void) = default;

    virtual void Run(Compositor &compositor) = 0;
protected:
    CompositorTask(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_TASK_H
