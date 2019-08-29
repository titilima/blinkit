// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_form_element.h
// Description: CrawlerFormElement Class
//      Author: Ziming Li
//     Created: 2019-08-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_FORM_ELEMENT_H
#define BLINKIT_BLINKIT_CRAWLER_FORM_ELEMENT_H

#pragma once

#include "core/loader/FormSubmission.h"
#include "crawler_element.h"

namespace BlinKit {

class CrawlerFormElement final : public CrawlerElement
{
public:
    static PassRefPtrWillBeRawPtr<CrawlerFormElement> Create(blink::Document &document) {
        return adoptRefWillBeNoop(new CrawlerFormElement(document));
    }

    void CollectFormData(blink::FormData &dst, const blink::Element &src);
    void Submit(void);
private:
    CrawlerFormElement(blink::Document &document);
    void CollectInputData(blink::FormData &dst, const blink::Element &src, const AtomicString &name);
    void parseAttribute(const blink::QualifiedName &name, const AtomicString &oldValue, const AtomicString &value) override;

    blink::FormSubmission::Attributes m_attributes;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_FORM_ELEMENT_H
