// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: DragEventInit.h
// Description: DragEventInit Class
//      Author: Ziming Li
//     Created: 2019-02-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DRAG_EVENT_INIT_H
#define BLINKIT_BLINK_DRAG_EVENT_INIT_H

#pragma once

#include "core/clipboard/DataTransfer.h"
#include "core/events/MouseEventInit.h"
#include "platform/heap/Handle.h"

namespace blink {

class DragEventInit : public MouseEventInit
{
public:
    DataTransfer* dataTransfer(void) const { return m_dataTransfer.get(); }
private:
    PersistentWillBeMember<DataTransfer> m_dataTransfer;
};

} // namespace blink

#endif // BLINKIT_BLINK_DRAG_EVENT_INIT_H
