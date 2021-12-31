#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_task.h
// Description: PaintTask Class
//      Author: Ziming Li
//     Created: 2021-12-02
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PAINT_TASK_H
#define BLINKIT_PAINT_TASK_H

namespace BlinKit {

class PaintTask
{
public:
    virtual void Run(void) = 0;
};

} // namespace BlinKit

#endif // BLINKIT_PAINT_TASK_H
