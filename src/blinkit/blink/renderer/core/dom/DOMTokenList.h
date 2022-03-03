#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMTokenList.h
// Description: DOMTokenList Class
//      Author: Ziming Li
//     Created: 2021-07-13
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

#ifndef DOMTokenList_h
#define DOMTokenList_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/wtf/Vector.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"

namespace blink {

class Element;
class ExceptionState;

class DOMTokenList : public ScriptWrappable
{
    WTF_MAKE_NONCOPYABLE(DOMTokenList);
public:
    DOMTokenList() { }
    virtual ~DOMTokenList() { }

    virtual unsigned length() const = 0;
    virtual const AtomicString item(unsigned index) const = 0;

    bool contains(const AtomicString&, ExceptionState&) const;
    virtual void add(const Vector<String>&, ExceptionState&);
    void add(const AtomicString&, ExceptionState&);
    virtual void remove(const Vector<String>&, ExceptionState&);
    void remove(const AtomicString&, ExceptionState&);
    bool toggle(const AtomicString&, ExceptionState&);
    bool toggle(const AtomicString&, bool force, ExceptionState&);
    bool supports(const AtomicString&, ExceptionState&);

    const AtomicString& toString() const { return value(); }

    virtual Element* element() { return 0; }
protected:

    virtual const AtomicString& value() const = 0;
    virtual void setValue(const AtomicString&) = 0;

    virtual void addInternal(const AtomicString&);
    virtual bool containsInternal(const AtomicString&) const = 0;
    virtual void removeInternal(const AtomicString&);

    bool validateToken(const String&, ExceptionState&) const;
    bool validateTokens(const Vector<String>&, ExceptionState&) const;
    virtual bool validateTokenValue(const AtomicString&, ExceptionState&) const;
    static AtomicString addToken(const AtomicString&, const AtomicString&);
    static AtomicString addTokens(const AtomicString&, const Vector<String>&);
    static AtomicString removeToken(const AtomicString&, const AtomicString&);
    static AtomicString removeTokens(const AtomicString&, const Vector<String>&);
};

} // namespace blink

#endif // DOMTokenList_h
