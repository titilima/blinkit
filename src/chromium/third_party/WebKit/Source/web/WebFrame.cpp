// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: WebFrame.cpp
// Description: WebFrame Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "public/web/WebFrame.h"

#include "core/frame/FrameHost.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/page/Page.h"
#include "platform/UserGestureIndicator.h"
#include "platform/heap/Handle.h"
#include "public/web/WebElement.h"
#include "public/web/WebFrameOwnerProperties.h"
#include "public/web/WebSandboxFlags.h"
#include "web/OpenedFrameTracker.h"
#include "web/WebLocalFrameImpl.h"
#include <algorithm>

namespace blink {

void WebFrame::detach()
{
    toImplBase()->frame()->detach(FrameDetachType::Remove);
}

bool WebFrame::shouldEnforceStrictMixedContentChecking() const
{
    return toImplBase()->frame()->securityContext()->shouldEnforceStrictMixedContentChecking();
}

WebFrame* WebFrame::opener() const
{
    return m_opener;
}

void WebFrame::setOpener(WebFrame* opener)
{
    if (m_opener)
        m_opener->m_openedFrameTracker->remove(this);
    if (opener)
        opener->m_openedFrameTracker->add(this);
    m_opener = opener;
}

bool WebFrame::isLoading() const
{
    if (Frame* frame = toImplBase()->frame())
        return frame->isLoading();
    return false;
}

WebFrame* WebFrame::fromFrame(Frame* frame)
{
    if (!frame)
        return 0;

    if (frame->isLocalFrame())
        return WebLocalFrameImpl::fromFrame(toLocalFrame(*frame));
    assert(false); // Not reached!
    return nullptr;
}

WebFrame::WebFrame(WebTreeScopeType scope)
    : m_scope(scope)
    , m_opener(0)
    , m_openedFrameTracker(new OpenedFrameTracker)
{
}

WebFrame::~WebFrame()
{
    m_openedFrameTracker.reset(0);
}

#if ENABLE(OILPAN)
ALWAYS_INLINE bool WebFrame::isFrameAlive(const WebFrame* frame)
{
    if (!frame)
        return true;

    if (frame->isWebLocalFrame())
        return Heap::isHeapObjectAlive(toWebLocalFrameImpl(frame));

    return Heap::isHeapObjectAlive(toWebRemoteFrameImpl(frame));
}

template <typename VisitorDispatcher>
ALWAYS_INLINE void WebFrame::traceFrameImpl(VisitorDispatcher visitor, WebFrame* frame)
{
    if (!frame)
        return;

    if (frame->isWebLocalFrame())
        visitor->trace(toWebLocalFrameImpl(frame));
    else
        visitor->trace(toWebRemoteFrameImpl(frame));
}

template <typename VisitorDispatcher>
ALWAYS_INLINE void WebFrame::traceFramesImpl(VisitorDispatcher visitor, WebFrame* frame)
{
    ASSERT(frame);
    traceFrame(visitor, frame->m_parent);
    for (WebFrame* child = frame->firstChild(); child; child = child->nextSibling())
        traceFrame(visitor, child);
    // m_opener is a weak reference.
    frame->m_openedFrameTracker->traceFrames(visitor);
}

template <typename VisitorDispatcher>
ALWAYS_INLINE void WebFrame::clearWeakFramesImpl(VisitorDispatcher visitor)
{
    if (!isFrameAlive(m_opener))
        m_opener = nullptr;
}

#define DEFINE_VISITOR_METHOD(VisitorDispatcher)                                                                               \
    void WebFrame::traceFrame(VisitorDispatcher visitor, WebFrame* frame) { traceFrameImpl(visitor, frame); }                  \
    void WebFrame::traceFrames(VisitorDispatcher visitor, WebFrame* frame) { traceFramesImpl(visitor, frame); }                \
    void WebFrame::clearWeakFrames(VisitorDispatcher visitor) { clearWeakFramesImpl(visitor); }

DEFINE_VISITOR_METHOD(Visitor*)
DEFINE_VISITOR_METHOD(InlinedGlobalMarkingVisitor)

#undef DEFINE_VISITOR_METHOD
#endif

} // namespace blink
