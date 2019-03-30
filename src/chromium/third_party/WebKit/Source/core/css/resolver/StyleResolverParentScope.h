// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: StyleResolverParentScope.h
// Description: StyleResolverParentScope Class
//      Author: Ziming Li
//     Created: 2019-03-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef StyleResolverParentScope_h
#define StyleResolverParentScope_h

#include "core/css/SelectorFilter.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/dom/shadow/ShadowRoot.h"

namespace blink {

// Maintains the parent element stack (and bloom filter) inside recalcStyle.
class StyleResolverParentScope final {
    STACK_ALLOCATED();
public:
    explicit StyleResolverParentScope(Node& parent);
    ~StyleResolverParentScope();

    static void ensureParentStackIsPushed();

private:
    void pushParentIfNeeded();
    Node& parent() const { return *m_parent; }

    RawPtrWillBeMember<Node> m_parent;
    bool m_pushed;
    StyleResolverParentScope* m_previous;
    RawPtrWillBeMember<StyleResolver> m_resolver;

    static StyleResolverParentScope* s_currentScope;
};

inline StyleResolverParentScope::StyleResolverParentScope(Node& parent)
    : m_parent(parent)
    , m_pushed(false)
    , m_previous(s_currentScope)
#ifndef BLINKIT_CRAWLER_ONLY
    , m_resolver(parent.document().styleResolver())
#endif
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    ASSERT(parent.document().inStyleRecalc());
    ASSERT(parent.isElementNode() || parent.isShadowRoot());
    s_currentScope = this;
    m_resolver->increaseStyleSharingDepth();
#endif
}

inline StyleResolverParentScope::~StyleResolverParentScope()
{
    s_currentScope = m_previous;
    m_resolver->decreaseStyleSharingDepth();
    if (!m_pushed)
        return;
    if (parent().isElementNode())
        m_resolver->selectorFilter().popParent(toElement(parent()));
}

inline void StyleResolverParentScope::ensureParentStackIsPushed()
{
    if (s_currentScope)
        s_currentScope->pushParentIfNeeded();
}

inline void StyleResolverParentScope::pushParentIfNeeded()
{
    if (m_pushed)
        return;
    if (m_previous)
        m_previous->pushParentIfNeeded();
    if (parent().isElementNode())
        m_resolver->selectorFilter().pushParent(toElement(parent()));
    m_pushed = true;
}

} // namespace blink

#endif // StyleResolverParentScope_h
