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

#include "blinkit/app/caller.h"
#include "blinkit/blink/renderer/core/dom/Document.h"
#include "blinkit/ui/web_view_impl.h"

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

    WebViewImpl *view = WebViewImpl::From(toDocument(*executionContext));
    auto task = [ob = m_ob, userData = m_userData] {
        ob(userData);
    };
    view->GetClientCaller().Post(BLINK_FROM_HERE, std::move(task));
}

} // namespace BlinKit
