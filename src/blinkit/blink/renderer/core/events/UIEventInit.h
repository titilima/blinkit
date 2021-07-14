// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: UIEventInit.h
// Description: UIEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_UI_EVENT_INIT_H
#define BLINKIT_BLINKIT_UI_EVENT_INIT_H

#pragma once

#include "blinkit/blink/renderer/core/events/EventInit.h"
#include "blinkit/blink/renderer/core/frame/DOMWindow.h"
#include "blinkit/blink/renderer/core/input/InputDeviceCapabilities.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"

namespace blink {

class UIEventInit : public EventInit
{
public:
    bool hasDetail(void) const { return true; }
    int detail(void) const { return m_detail; }
    void setDetail(int value) { m_detail = value; }

    bool hasView(void) const { return m_view; }
    PassRefPtrWillBeRawPtr<DOMWindow> view(void) const { return m_view; }
    void setView(PassRefPtrWillBeRawPtr<DOMWindow> value) { m_view = value; }

    bool hasSourceCapabilities(void) const { return m_sourceCapabilities; }
    InputDeviceCapabilities* sourceCapabilities(void) const { return m_sourceCapabilities; }
    void setSourceCapabilities(InputDeviceCapabilities *value) { m_sourceCapabilities = value; }
private:
    int m_detail = 0;
    RefPtrWillBeMember<DOMWindow> m_view;
    Member<InputDeviceCapabilities> m_sourceCapabilities;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_UI_EVENT_INIT_H
