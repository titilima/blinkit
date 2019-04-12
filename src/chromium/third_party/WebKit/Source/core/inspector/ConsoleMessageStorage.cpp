// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ConsoleMessageStorage.cpp
// Description: Placeholder for ConsoleMessageStorage
//      Author: Ziming Li
//     Created: 2019-02-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "core/inspector/ConsoleMessageStorage.h"

namespace blink {

void ConsoleMessageStorage::clear(ExecutionContext *)
{
    // Nothing to do, just a placeholder.
}

void ConsoleMessageStorage::reportMessage(ExecutionContext *, PassRefPtrWillBeRawPtr<ConsoleMessage> prpMessage)
{
#ifdef _DEBUG
    assert(false); // BKTODO: Show log for the message.
#endif
}

} // namespace blink
