// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FontFaceSetLoadEventInit.h
// Description: FontFaceSetLoadEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-30
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FONT_FACE_SET_LOAD_EVENT_INIT_H
#define BLINKIT_BLINK_FONT_FACE_SET_LOAD_EVENT_INIT_H

#pragma once

#include <optional>
#include "core/css/FontFace.h"
#include "core/events/EventInit.h"

namespace blink {

class FontFaceSetLoadEventInit : public EventInit
{
public:
    bool hasFontfaces(void) const { return m_fontfaces.has_value(); }
    const WillBeHeapVector<RefPtrWillBeMember<FontFace>>& fontfaces() const { return m_fontfaces.value(); }
    void setFontfaces(const WillBeHeapVector<RefPtrWillBeMember<FontFace>> &value) { m_fontfaces = value; }
private:
    std::optional<WillBeHeapVector<RefPtrWillBeMember<FontFace>>> m_fontfaces;
};

} // namespace blink

#endif // BLINKIT_BLINK_FONT_FACE_SET_LOAD_EVENT_INIT_H
