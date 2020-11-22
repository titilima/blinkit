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

#include "blinkit/crawler/dom/crawler_script_element.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/script/script_loader.h"
#ifndef BLINKIT_CRAWLER_ONLY
// BKTODO: #include "third_party/blink/renderer/core/html/html_script_element.h"
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

void ScriptElementBase::DispatchErrorEvent(void)
{
    GetElement().DispatchEvent(*Event::Create(event_type_names::kError));
}

void ScriptElementBase::DispatchLoadEvent(void)
{
    GetElement().DispatchEvent(*Event::Create(event_type_names::kLoad));
}

String ScriptElementBase::EventAttributeValue(void) const
{
    return GetElement().getAttribute(kEventAttr).GetString();
}

String ScriptElementBase::ForAttributeValue(void) const
{
    return GetElement().getAttribute(kForAttr).GetString();
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

bool ScriptElementBase::IsURLAttributeImpl(const QualifiedName &attrName)
{
    using namespace html_names;
    if (attrName == kSrcAttr)
        return true;
    return false;
}

String ScriptElementBase::LanguageAttributeValue(void) const
{
    return GetElement().getAttribute(kLanguageAttr).GetString();
}

bool ScriptElementBase::NomoduleAttributeValue(void) const
{
    return GetElement().FastHasAttribute(kNomoduleAttr);
}

bool ScriptElementBase::ParseAttributeImpl(const Element::AttributeModificationParams &params)
{
    if (params.name == kSrcAttr)
        m_loader->HandleSourceAttribute(params.new_value);
    else
        return false;
    return true;
}

String ScriptElementBase::SourceAttributeValue(void) const
{
    return GetElement().getAttribute(kSrcAttr).GetString();
}

String ScriptElementBase::TextFromChildren(void) const
{
    return GetElement().TextFromChildren();
}

void ScriptElementBase::Trace(Visitor *visitor)
{
    m_loader->Trace(visitor);
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
