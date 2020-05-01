// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_script_element.cpp
// Description: CrawlerScriptElement Class
//      Author: Ziming Li
//     Created: 2019-04-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_script_element.h"

#include "third_party/blink/renderer/core/html_names.h"

using namespace blink;

namespace BlinKit {

CrawlerScriptElement::CrawlerScriptElement(Document &document, const CreateElementFlags flags)
    : CrawlerElement(html_names::kScriptTag.LocalName(), &document)
    , ScriptElementBase(flags.IsCreatedByParser(), flags.WasAlreadyStarted())
{
}

void CrawlerScriptElement::ChildrenChanged(const ChildrenChange &change)
{
    CrawlerElement::ChildrenChanged(change);
    ScriptElementBase::ChildrenChangedImpl(change);
}

void CrawlerScriptElement::DidNotifySubtreeInsertionsToDocument(void)
{
    ScriptElementBase::DidNotifySubtreeInsertionsToDocumentImpl();
}

Node::InsertionNotificationRequest CrawlerScriptElement::InsertedInto(ContainerNode &insertionPoint)
{
    CrawlerElement::InsertedInto(insertionPoint);
    return ScriptElementBase::InsertedIntoImpl(insertionPoint);
}

bool CrawlerScriptElement::IsURLAttribute(const Attribute &attribute) const
{
    if (ScriptElementBase::IsURLAttributeImpl(attribute.GetName()))
        return true;
    return CrawlerElement::IsURLAttribute(attribute);
}

void CrawlerScriptElement::ParseAttribute(const AttributeModificationParams &params)
{
    if (!ScriptElementBase::ParseAttributeImpl(params))
        CrawlerElement::ParseAttribute(params);
}

} // namespace BlinKit
