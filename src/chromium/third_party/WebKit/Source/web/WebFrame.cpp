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
#include "core/html/HTMLFrameElementBase.h"
#include "core/html/HTMLFrameOwnerElement.h"
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

void WebFrame::insertAfter(WebFrame* newChild, WebFrame* previousSibling)
{
    newChild->m_parent = this;

    WebFrame* next;
    if (!previousSibling) {
        // Insert at the beginning if no previous sibling is specified.
        next = m_firstChild;
        m_firstChild = newChild;
    } else {
        ASSERT(previousSibling->m_parent == this);
        next = previousSibling->m_nextSibling;
        previousSibling->m_nextSibling = newChild;
        newChild->m_previousSibling = previousSibling;
    }

    if (next) {
        newChild->m_nextSibling = next;
        next->m_previousSibling = newChild;
    } else {
        m_lastChild = newChild;
    }

    toImplBase()->frame()->tree().invalidateScopedChildCount();
    toImplBase()->frame()->host()->incrementSubframeCount();
}

void WebFrame::appendChild(WebFrame* child)
{
    // TODO(dcheng): Original code asserts that the frames have the same Page.
    // We should add an equivalent check... figure out what.
    insertAfter(child, m_lastChild);
}

void WebFrame::removeChild(WebFrame* child)
{
    child->m_parent = 0;

    if (m_firstChild == child)
        m_firstChild = child->m_nextSibling;
    else
        child->m_previousSibling->m_nextSibling = child->m_nextSibling;

    if (m_lastChild == child)
        m_lastChild = child->m_previousSibling;
    else
        child->m_nextSibling->m_previousSibling = child->m_previousSibling;

    child->m_previousSibling = child->m_nextSibling = 0;

    toImplBase()->frame()->tree().invalidateScopedChildCount();
    toImplBase()->frame()->host()->decrementSubframeCount();
}

void WebFrame::setParent(WebFrame* parent)
{
    m_parent = parent;
}

WebFrame* WebFrame::parent() const
{
    return m_parent;
}

WebFrame* WebFrame::top() const
{
    WebFrame* frame = const_cast<WebFrame*>(this);
    for (WebFrame* parent = frame; parent; parent = parent->m_parent)
        frame = parent;
    return frame;
}

WebFrame* WebFrame::firstChild() const
{
    return m_firstChild;
}

WebFrame* WebFrame::lastChild() const
{
    return m_lastChild;
}

WebFrame* WebFrame::previousSibling() const
{
    return m_previousSibling;
}

WebFrame* WebFrame::nextSibling() const
{
    return m_nextSibling;
}

WebFrame* WebFrame::traversePrevious(bool wrap) const
{
    if (Frame* frame = toImplBase()->frame())
        return fromFrame(frame->tree().traversePreviousWithWrap(wrap));
    return 0;
}

WebFrame* WebFrame::traverseNext(bool wrap) const
{
    if (Frame* frame = toImplBase()->frame())
        return fromFrame(frame->tree().traverseNextWithWrap(wrap));
    return 0;
}

WebFrame* WebFrame::findChildByName(const WebString& name) const
{
    Frame* frame = toImplBase()->frame();
    if (!frame)
        return 0;
    // FIXME: It's not clear this should ever be called to find a remote frame.
    // Perhaps just disallow that completely?
    return fromFrame(frame->tree().child(name));
}

WebFrame* WebFrame::fromFrameOwnerElement(const WebElement& webElement)
{
    Element* element = PassRefPtrWillBeRawPtr<Element>(webElement).get();

    if (!element->isFrameOwnerElement())
        return nullptr;
    return fromFrame(toHTMLFrameOwnerElement(element)->contentFrame());
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
    , m_parent(0)
    , m_previousSibling(0)
    , m_nextSibling(0)
    , m_firstChild(0)
    , m_lastChild(0)
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
