#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: click_observer_wrapper.h
// Description: ClickObserverWrapper Class
//      Author: Ziming Li
//     Created: 2021-11-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLICK_OBSERVER_WRAPPER_H
#define BLINKIT_BLINKIT_CLICK_OBSERVER_WRAPPER_H

#include "bk_ui.h"
#include "blinkit/blink/renderer/core/events/EventListener.h"

namespace BlinKit {

class ClickObserverWrapper final : public blink::EventListener
{
public:
    static GCRefPtr<ClickObserverWrapper> Create(BkClickObserver ob, void *userData);
private:
    ClickObserverWrapper(BkClickObserver ob, void *userData);

    // blink::EventListener
    bool operator==(const blink::EventListener &other) const override { return this == &other; }
    void handleEvent(blink::ExecutionContext *executionContext, blink::Event *event) override;

    BkClickObserver m_ob;
    void *m_userData;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLICK_OBSERVER_WRAPPER_H
