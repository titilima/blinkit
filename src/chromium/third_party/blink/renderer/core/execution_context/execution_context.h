// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: execution_context.h
// Description: ExecutionContext Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EXECUTION_CONTEXT_H
#define BLINKIT_BLINK_EXECUTION_CONTEXT_H

#pragma once

#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/dom/context_lifecycle_notifier.h"
#include "third_party/blink/renderer/core/dom/context_lifecycle_observer.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

enum ReasonForCallingCanExecuteScripts {
    kAboutToExecuteScript,
    kNotAboutToExecuteScript
};

class ExecutionContext : public ContextLifecycleNotifier
{
public:
    virtual bool IsDocument(void) const { return false; }

    virtual bool CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) { return false; }

    virtual std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_EXECUTION_CONTEXT_H
