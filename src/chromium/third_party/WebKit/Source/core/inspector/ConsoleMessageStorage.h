// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ConsoleMessageStorage.h
// Description: Placeholder for ConsoleMessageStorage
//      Author: Ziming Li
//     Created: 2019-02-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_CONSOLE_MESSAGE_STORAGE_H
#define BLINKIT_BLINK_CONSOLE_MESSAGE_STORAGE_H

#include "core/inspector/ConsoleMessage.h"
#include "platform/heap/Handle.h"

namespace blink {

class ExecutionContext;
class LocalDOMWindow;

class ConsoleMessageStorage final : public NoBaseWillBeGarbageCollected<ConsoleMessageStorage> {
    WTF_MAKE_NONCOPYABLE(ConsoleMessageStorage);
    USING_FAST_MALLOC_WILL_BE_REMOVED(ConsoleMessageStorage);
public:
    static PassOwnPtrWillBeRawPtr<ConsoleMessageStorage> create()
    {
        return adoptPtrWillBeNoop(new ConsoleMessageStorage());
    }

    void reportMessage(ExecutionContext*, PassRefPtrWillBeRawPtr<ConsoleMessage>);
    void clear(ExecutionContext*);
private:
    ConsoleMessageStorage(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINK_CONSOLE_MESSAGE_STORAGE_H
