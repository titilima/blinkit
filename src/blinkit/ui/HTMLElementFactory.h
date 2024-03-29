// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HTMLElementFactory_h
#define HTMLElementFactory_h

#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Forward.h"

namespace blink {

class Document;
class HTMLElement;
class HTMLFormElement;

class HTMLElementFactory
{
public:
    static GCRefPtr<HTMLElement> createHTMLElement(
        const AtomicString& localName,
        Document&,
        HTMLFormElement* = 0,
        bool createdByParser = true);
};

} // namespace blink

#endif
