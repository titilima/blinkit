// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_timeline_options.h
// Description: DocumentTimelineOptions Class
//      Author: Ziming Li
//     Created: 2020-09-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DOCUMENT_TIMELINE_OPTIONS_H
#define BLINKIT_BLINK_DOCUMENT_TIMELINE_OPTIONS_H

#pragma once

namespace blink {

class DocumentTimelineOptions
{
public:
    double originTime(void) const { return m_originTime; }
private:
    double m_originTime{ 0.0 };
};

} // namespace blink

#endif // BLINKIT_BLINK_DOCUMENT_TIMELINE_OPTIONS_H
