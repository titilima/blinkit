// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: message_task.h
// Description: MessageTask Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MESSAGE_TASK_H
#define BLINKIT_BLINKIT_MESSAGE_TASK_H

#pragma once

#include <functional>

namespace BlinKit {

class MessageTask
{
public:
    using TaskType = std::function<void()>;

    static bool Post(DWORD threadId, TaskType &&task);
    static bool Post(HWND hWnd, TaskType &&task);

    static bool Process(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MESSAGE_TASK_H
