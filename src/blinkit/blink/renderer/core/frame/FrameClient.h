// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FrameClient.h
// Description: FrameClient Class
//      Author: Ziming Li
//     Created: 2021-08-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FrameClient_h
#define FrameClient_h

#include "core/CoreExport.h"
#include "platform/heap/Handle.h"

namespace blink {

class Frame;
class LocalFrame;

class CORE_EXPORT FrameClient
{
public:
    // BKTODO: virtual bool inShadowTree() const = 0;
    enum class Type { Crawler, WebView };
#if defined(BLINKIT_CRAWLER_ONLY)
    constexpr Type GetType(void) const { return Type::Crawler; }
#elif defined(BLINKIT_UI_ONLY)
    constexpr Type GetType(void) const { return Type::WebView; }
#else
    virtual Type GetType(void) const = 0;
#endif

    // TODO(dcheng): Move this into FrameLoaderClient, since remote frames don't
    // need this.
    virtual void willBeDetached() {}
    virtual void detached() {}

#if 0 // BKTODO:
    virtual Frame* opener() const = 0;
    virtual void setOpener(Frame*) = 0;

    virtual Frame* parent() const = 0;
    virtual Frame* top() const = 0;
    virtual Frame* previousSibling() const = 0;
    virtual Frame* nextSibling() const = 0;
    virtual Frame* firstChild() const = 0;
    virtual Frame* lastChild() const = 0;

    virtual unsigned backForwardLength() = 0;

    // Returns true if the embedder intercepted the postMessage call
    virtual bool willCheckAndDispatchMessageEvent(SecurityOrigin* /*target*/, MessageEvent*, LocalFrame* /*sourceFrame*/) const { return false; }
#endif

    virtual void frameFocused() const {}

    virtual ~FrameClient() { }

    DEFINE_INLINE_VIRTUAL_TRACE() { }
};

} // namespace blink

#endif // FrameClient_h
