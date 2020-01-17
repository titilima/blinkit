// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_element_base.cpp
// Description: ScriptElementBase Class
//      Author: Ziming Li
//     Created: 2020-01-13
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "script_element_base.h"

#include "blinkit/crawler/crawler_script_element.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/script/script_loader.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/html/html_script_element.h"
#endif

using namespace BlinKit;

namespace blink {

using namespace html_names;

ScriptElementBase::ScriptElementBase(bool parserInserted, bool alreadyStarted)
    : m_loader(ScriptLoader::Create(this, parserInserted, alreadyStarted))
{
}

ScriptElementBase::~ScriptElementBase(void) = default;

String ScriptElementBase::CharsetAttributeValue(void) const
{
    return GetElement().getAttribute(kCharsetAttr).GetString();
}

void ScriptElementBase::ChildrenChangedImpl(const ContainerNode::ChildrenChange &change)
{
    if (change.IsChildInsertion())
        m_loader->ChildrenChanged();
}

#if 0
String ScriptElementBase::ScriptElementBaseEventAttributeValue(void) const
{
}
#endif

void ScriptElementBase::DidNotifySubtreeInsertionsToDocumentImpl(void)
{
    m_loader->DidNotifySubtreeInsertionsToDocument();
}

bool ScriptElementBase::HasSourceAttribute(void) const
{
    return GetElement().FastHasAttribute(kSrcAttr);
}

Node::InsertionNotificationRequest ScriptElementBase::InsertedIntoImpl(ContainerNode &insertionPoint)
{
    return Node::kInsertionShouldCallDidNotifySubtreeInsertions;
}

bool ScriptElementBase::IsConnected(void) const
{
    return GetElement().isConnected();
}

String ScriptElementBase::LanguageAttributeValue(void) const
{
    return GetElement().getAttribute(kLanguageAttr).GetString();
}

bool ScriptElementBase::ParseAttributeImpl(const Element::AttributeModificationParams &params)
{
    if (params.name == kSrcAttr)
        m_loader->HandleSourceAttribute(params.newValue);
    else
        return false;
    return true;
}

String ScriptElementBase::TextFromChildren(void) const
{
    return GetElement().TextFromChildren();
}

String ScriptElementBase::TypeAttributeValue(void) const
{
    return GetElement().getAttribute(kTypeAttr).GetString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScriptLoader* ScriptLoaderFromElement(Element *element)
{
    if (element->HasTagName(kScriptTag))
    {
        if (element->ForCrawler())
            return static_cast<CrawlerScriptElement *>(element)->Loader();
#ifndef BLINKIT_CRAWLER_ONLY
        ASSERT(false); // BKTODO:
#endif
    }
    ASSERT(false); // Not reached!
    return nullptr;
}

}  // namespace blink
