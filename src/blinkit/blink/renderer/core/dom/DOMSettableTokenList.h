#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMSettableTokenList.h
// Description: DOMSettableTokenList Class
//      Author: Ziming Li
//     Created: 2021-09-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DOMSettableTokenList_h
#define DOMSettableTokenList_h

#include "blinkit/blink/renderer/core/dom/DOMTokenList.h"
#include "blinkit/blink/renderer/core/dom/SpaceSplitString.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"

namespace blink {

class ExceptionState;

class DOMSettableTokenListObserver : public WillBeGarbageCollectedMixin
{
public:
    // Called when the value property is set, even if the tokens in
    // the set have not changed.
    virtual void valueWasSet() = 0;

    DEFINE_INLINE_VIRTUAL_TRACE() { }
};

class DOMSettableTokenList
    : public DOMTokenList
#if !ENABLE(OILPAN)
    , public RefCounted<DOMSettableTokenList>
#endif
    {
    DEFINE_WRAPPERTYPEINFO();
    USING_FAST_MALLOC_WILL_BE_REMOVED(DOMSettableTokenList);
public:
    static std::unique_ptr<DOMSettableTokenList> create(DOMSettableTokenListObserver* observer = nullptr)
    {
        return zed::wrap_unique(new DOMSettableTokenList(observer));
    }
    ~DOMSettableTokenList() override;

#if !ENABLE(OILPAN)
    void ref() override { RefCounted<DOMSettableTokenList>::ref(); }
    void deref() override { RefCounted<DOMSettableTokenList>::deref(); }
#endif

    unsigned length() const override { return m_tokens.size(); }
    const AtomicString item(unsigned index) const override;

    const AtomicString& value() const override { return m_value; }
    void setValue(const AtomicString&) override;

    const SpaceSplitString& tokens() const { return m_tokens; }
    void setObserver(DOMSettableTokenListObserver* observer) { m_observer = observer; }

    // BKTODO: DECLARE_VIRTUAL_TRACE();

protected:
    explicit DOMSettableTokenList(DOMSettableTokenListObserver*);

private:
    bool containsInternal(const AtomicString&) const override;

    AtomicString m_value;
    SpaceSplitString m_tokens;
    DOMSettableTokenListObserver *m_observer;
};

} // namespace blink

#endif // DOMSettableTokenList_h
