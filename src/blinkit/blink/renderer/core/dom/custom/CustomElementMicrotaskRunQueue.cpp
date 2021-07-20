// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CustomElementMicrotaskRunQueue.cpp
// Description: CustomElementMicrotaskRunQueue Class
//      Author: Ziming Li
//     Created: 2021-07-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/custom/CustomElementMicrotaskRunQueue.h"

#if 0 // BKTODO:
#include "core/dom/Microtask.h"
#include "core/dom/custom/CustomElementAsyncImportMicrotaskQueue.h"
#include "core/dom/custom/CustomElementSyncMicrotaskQueue.h"
#endif
#include "core/html/imports/HTMLImportLoader.h"

namespace blink {

DEFINE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(CustomElementMicrotaskRunQueue)

CustomElementMicrotaskRunQueue::CustomElementMicrotaskRunQueue()
#if 0 // BKTODO:
    : m_syncQueue(CustomElementSyncMicrotaskQueue::create())
    , m_asyncQueue(CustomElementAsyncImportMicrotaskQueue::create())
    , m_dispatchIsPending(false)
#else
    : m_dispatchIsPending(false)
#endif
#if !ENABLE(OILPAN)
    , m_weakFactory(this)
#endif
{
}

void CustomElementMicrotaskRunQueue::enqueue(HTMLImportLoader* parentLoader, PassOwnPtrWillBeRawPtr<CustomElementMicrotaskStep> step, bool importIsSync)
{
    ASSERT(false); // BKTODO:
#if 0
    if (importIsSync) {
        if (parentLoader)
            parentLoader->microtaskQueue()->enqueue(step);
        else
            m_syncQueue->enqueue(step);
    } else {
        m_asyncQueue->enqueue(step);
    }
#endif

    requestDispatchIfNeeded();
}

#if 0 // BKTODO:
void CustomElementMicrotaskRunQueue::dispatchIfAlive(WeakPtrWillBeWeakPersistent<CustomElementMicrotaskRunQueue> self)
{
    if (self.get()) {
        RefPtrWillBeRawPtr<CustomElementMicrotaskRunQueue> protect(self.get());
        self->dispatch();
    }
}
#endif

void CustomElementMicrotaskRunQueue::requestDispatchIfNeeded()
{
    if (m_dispatchIsPending || isEmpty())
        return;
#if ENABLE(OILPAN)
    ASSERT(false); // BKTODO: Microtask::enqueueMicrotask(WTF::bind(&CustomElementMicrotaskRunQueue::dispatchIfAlive, WeakPersistent<CustomElementMicrotaskRunQueue>(this)));
#else
    Microtask::enqueueMicrotask(WTF::bind(&CustomElementMicrotaskRunQueue::dispatchIfAlive, m_weakFactory.createWeakPtr()));
#endif
    m_dispatchIsPending = true;
}

DEFINE_TRACE(CustomElementMicrotaskRunQueue)
{
    visitor->trace(m_syncQueue);
    visitor->trace(m_asyncQueue);
}

void CustomElementMicrotaskRunQueue::dispatch()
{
    m_dispatchIsPending = false;
    ASSERT(false); // BKTODO:
#if 0
    m_syncQueue->dispatch();
    if (m_syncQueue->isEmpty())
        m_asyncQueue->dispatch();
#endif
}

bool CustomElementMicrotaskRunQueue::isEmpty() const
{
    ASSERT(false); // BKTODO: return m_syncQueue->isEmpty() && m_asyncQueue->isEmpty();
    return true;
}

} // namespace blink
