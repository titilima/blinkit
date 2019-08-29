// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_form_element.cpp
// Description: CrawlerFormElement Class
//      Author: Ziming Li
//     Created: 2019-08-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_form_element.h"

#include "core/InputTypeNames.h"
#include "core/events/Event.h"
#include "core/html/FormData.h"
#include "core/loader/FrameLoadRequest.h"

using namespace blink;

namespace BlinKit {

CrawlerFormElement::CrawlerFormElement(Document &document) : CrawlerElement(HTMLNames::formTag.localName(), &document)
{
    // Nothing
}

void CrawlerFormElement::CollectFormData(FormData &dst, const Element &src)
{
    const AtomicString &name = src.getAttribute(HTMLNames::nameAttr);
    if (name.isEmpty())
        return;

    if (src.hasTagName(HTMLNames::inputTag))
        return CollectInputData(dst, src, name);

#ifdef _DEBUG
    const std::string s = src.tagName().to_string();
    BKLOG("unexpected tag `%s`!", s.c_str());
#endif
    assert(false); // BKTODO:
}

void CrawlerFormElement::CollectInputData(FormData &dst, const Element &src, const AtomicString &name)
{
    const AtomicString &type = src.getAttribute(HTMLNames::typeAttr);
    if (type == InputTypeNames::text || type == InputTypeNames::password || type == InputTypeNames::hidden)
    {
        const AtomicString &value = src.getAttribute(HTMLNames::valueAttr);
        dst.append(name, value);
    }
    else
    {
#ifdef _DEBUG
        const std::string s = type.to_string();
        BKLOG("unexpected type `%s`!", s.c_str());
#endif
        assert(false); // BKTODO:
    }
}

void CrawlerFormElement::parseAttribute(const QualifiedName &name, const AtomicString &oldValue, const AtomicString &value)
{
    using namespace HTMLNames;
    if (name == actionAttr)
        m_attributes.parseAction(value);
    else if (name == targetAttr)
        m_attributes.setTarget(value);
    else if (name == methodAttr)
        m_attributes.updateMethodType(value);
    else if (name == enctypeAttr)
        m_attributes.updateEncodingType(value);
    else if (name == accept_charsetAttr)
        m_attributes.setAcceptCharset(value);
    else
        CrawlerElement::parseAttribute(name, oldValue, value);
}

void CrawlerFormElement::Submit(void)
{
    RefPtrWillBeRawPtr<FormSubmission> formSubmission = FormSubmission::create(this, m_attributes, nullptr);

    Document &document = this->document();

    FrameLoadRequest frameRequest(&document);
    formSubmission->populateFrameLoadRequest(frameRequest);
    frameRequest.setForm(this);
    document.frame()->loader().load(frameRequest);
}

} // namespace BlinKit
