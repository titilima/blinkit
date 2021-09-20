// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: RelList.h
// Description: RelList Class
//      Author: Ziming Li
//     Created: 2021-09-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RelList_h
#define RelList_h

#include "core/HTMLNames.h"
#include "core/dom/DOMTokenList.h"
#include "core/dom/Element.h"
#include "core/dom/SpaceSplitString.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

class RelList final : public DOMTokenList {
public:
    static std::unique_ptr<RelList> create(Element* element)
    {
        return zed::wrap_unique(new RelList(element));
    }

#if !ENABLE(OILPAN)
    void ref() override;
    void deref() override;
#endif

    unsigned length() const override;
    const AtomicString item(unsigned index) const override;

    Element* element() override { return m_element; }
    void setRelValues(const AtomicString&);

    // BKTODO: DECLARE_VIRTUAL_TRACE();

    using SupportedTokens = HashSet<AtomicString>;

private:
    explicit RelList(Element*);

    bool containsInternal(const AtomicString&) const override;

    const AtomicString& value() const override { return m_element->getAttribute(HTMLNames::relAttr); }
    void setValue(const AtomicString& value) override { m_element->setAttribute(HTMLNames::relAttr, value); }

    bool validateTokenValue(const AtomicString&, ExceptionState&) const override;

    Element *m_element;
    SpaceSplitString m_relValues;
};

} // namespace blink

#endif // RelList_h
