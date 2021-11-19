// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: click_observer_wrapper.cpp
// Description: ClickObserverWrapper Class
//      Author: Ziming Li
//     Created: 2021-11-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./click_observer_wrapper.h"

#include "blinkit/blink/renderer/core/dom/Document.h"

using namespace blink;

namespace BlinKit {

ClickObserverWrapper::ClickObserverWrapper(BkClickObserver ob, void *userData)
    : EventListener(CPPEventListenerType), m_ob(ob), m_userData(userData)
{
}

GCRefPtr<ClickObserverWrapper> ClickObserverWrapper::Create(BkClickObserver ob, void *userData)
{
    return GCWrapShared(new ClickObserverWrapper(ob, userData));
}

void ClickObserverWrapper::handleEvent(ExecutionContext *executionContext, Event *)
{
    ASSERT(executionContext->isDocument());
    m_ob(m_userData);
}

} // namespace BlinKit
