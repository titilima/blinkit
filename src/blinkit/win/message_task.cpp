// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: message_task.cpp
// Description: MessageTask Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "message_task.h"

namespace BlinKit {

static const UINT TaskMessage = RegisterWindowMessage(TEXT("BkTaskMessage"));

bool MessageTask::Post(DWORD threadId, TaskType &&task)
{
    TaskType *p = new TaskType(std::move(task));
    if (PostThreadMessage(threadId, TaskMessage, 0, reinterpret_cast<LPARAM>(p)))
        return true;

    ASSERT(false); // Post failed!
    delete p;
    return false;
}

bool MessageTask::Post(HWND hWnd, TaskType &&task)
{
    TaskType *p = new TaskType(std::move(task));
    if (PostMessage(hWnd, TaskMessage, 0, reinterpret_cast<LPARAM>(p)))
        return true;

    ASSERT(false); // Post failed!
    delete p;
    return false;
}

bool MessageTask::Process(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (TaskMessage != uMsg)
        return false;

    TaskType *p = reinterpret_cast<TaskType *>(lParam);
    (*p)();
    delete p;
    return true;
}

} // namespace BlinKit
