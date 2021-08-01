// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaQueryListEventInit.h
// Description: MediaQueryListEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-30
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MEDIA_QUERY_LIST_EVENT_INIT_H
#define BLINKIT_BLINK_MEDIA_QUERY_LIST_EVENT_INIT_H

#pragma once

#include <optional>
#include "blinkit/blink/renderer/core/events/EventInit.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class MediaQueryListEventInit : public EventInit
{
public:
    bool hasMatches(void) const { return m_matches.has_value(); }
    bool matches(void) const { return m_matches.value(); }
    void setMatches(bool value) { m_matches = value; }

    bool hasMedia(void) const { return !m_media.isNull(); }
    const String& media(void) const { return m_media; }
    void setMedia(const String &value) { m_media = value; }
private:
    std::optional<bool> m_matches;
    String m_media;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_MEDIA_QUERY_LIST_EVENT_INIT_H
