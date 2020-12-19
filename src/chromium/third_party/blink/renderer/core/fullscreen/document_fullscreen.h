// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_fullscreen.h
// Description: Placeholder for Fullscreen Stuff
//      Author: Ziming Li
//     Created: 2020-12-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DOCUMENT_FULLSCREEN_H
#define BLINKIT_BLINK_DOCUMENT_FULLSCREEN_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class Document;
class Element;

class DocumentFullscreen
{
    STATIC_ONLY(DocumentFullscreen);
public:
    static constexpr Element* fullscreenElement(Document &) { return nullptr; }
};

} // namespace blink

#endif // BLINKIT_BLINK_DOCUMENT_FULLSCREEN_H
