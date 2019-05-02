// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FrameClient.h
// Description: FrameClient Class
//      Author: Ziming Li
//     Created: 2019-05-02
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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
class MessageEvent;
class SecurityOrigin;
enum class FrameDetachType;

class CORE_EXPORT FrameClient : public NoBaseWillBeGarbageCollectedFinalized<FrameClient> {
    USING_FAST_MALLOC_WILL_BE_REMOVED(FrameClient);
public:
    virtual bool inShadowTree() const = 0;
    virtual bool IsCrawler(void) const { return false; }

    // TODO(dcheng): Move this into FrameLoaderClient, since remote frames don't
    // need this.
    virtual void willBeDetached() = 0;
    virtual void detached(FrameDetachType) = 0;

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

    virtual void frameFocused() const = 0;

    virtual ~FrameClient() { }

    DEFINE_INLINE_VIRTUAL_TRACE() { }
};

} // namespace blink

#endif // FrameClient_h
