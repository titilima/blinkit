// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLTreeBuilderSimulator.cpp
// Description: HTMLTreeBuilderSimulator Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL GOOGLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/html/parser/HTMLTreeBuilderSimulator.h"

#include "core/HTMLNames.h"
#if 0 // BKTODO:
#include "core/MathMLNames.h"
#include "core/SVGNames.h"
#endif
#include "core/html/parser/HTMLParserIdioms.h"
#include "core/html/parser/HTMLTokenizer.h"
#include "core/html/parser/HTMLTreeBuilder.h"

namespace blink {

using namespace HTMLNames;

static bool tokenExitsForeignContent(const CompactHTMLToken& token)
{
    // FIXME: This is copied from HTMLTreeBuilder::processTokenInForeignContent and changed to use threadSafeHTMLNamesMatch.
    const String& tagName = token.data();
    return threadSafeMatch(tagName, bTag)
        || threadSafeMatch(tagName, bigTag)
        || threadSafeMatch(tagName, blockquoteTag)
        || threadSafeMatch(tagName, bodyTag)
        || threadSafeMatch(tagName, brTag)
        || threadSafeMatch(tagName, centerTag)
        || threadSafeMatch(tagName, codeTag)
        || threadSafeMatch(tagName, ddTag)
        || threadSafeMatch(tagName, divTag)
        || threadSafeMatch(tagName, dlTag)
        || threadSafeMatch(tagName, dtTag)
        || threadSafeMatch(tagName, emTag)
        // BKTODO: || threadSafeMatch(tagName, embedTag)
        || threadSafeMatch(tagName, h1Tag)
        || threadSafeMatch(tagName, h2Tag)
        || threadSafeMatch(tagName, h3Tag)
        || threadSafeMatch(tagName, h4Tag)
        || threadSafeMatch(tagName, h5Tag)
        || threadSafeMatch(tagName, h6Tag)
        || threadSafeMatch(tagName, headTag)
        || threadSafeMatch(tagName, hrTag)
        || threadSafeMatch(tagName, iTag)
        || threadSafeMatch(tagName, imgTag)
        || threadSafeMatch(tagName, liTag)
        || threadSafeMatch(tagName, listingTag)
        || threadSafeMatch(tagName, menuTag)
        || threadSafeMatch(tagName, metaTag)
        || threadSafeMatch(tagName, nobrTag)
        || threadSafeMatch(tagName, olTag)
        || threadSafeMatch(tagName, pTag)
        || threadSafeMatch(tagName, preTag)
        // BKTODO: || threadSafeMatch(tagName, rubyTag)
        || threadSafeMatch(tagName, sTag)
        || threadSafeMatch(tagName, smallTag)
        || threadSafeMatch(tagName, spanTag)
        || threadSafeMatch(tagName, strongTag)
        || threadSafeMatch(tagName, strikeTag)
        || threadSafeMatch(tagName, subTag)
        || threadSafeMatch(tagName, supTag)
        || threadSafeMatch(tagName, tableTag)
        || threadSafeMatch(tagName, ttTag)
        || threadSafeMatch(tagName, uTag)
        || threadSafeMatch(tagName, ulTag)
        || threadSafeMatch(tagName, varTag)
        || (threadSafeMatch(tagName, fontTag) && (token.getAttributeItem(colorAttr) || token.getAttributeItem(faceAttr) || token.getAttributeItem(sizeAttr)));
}

static bool tokenExitsSVG(const CompactHTMLToken& token)
{
    // FIXME: It's very fragile that we special case foreignObject here to be case-insensitive.
    ASSERT(false); // BKTODO: return equalIgnoringCaseNonNull(token.data().impl(), SVGNames::foreignObjectTag.localName().impl());
    return false;
}

static bool tokenExitsMath(const CompactHTMLToken& token)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    // FIXME: This is copied from HTMLElementStack::isMathMLTextIntegrationPoint and changed to use threadSafeMatch.
    const String& tagName = token.data();
    return threadSafeMatch(tagName, MathMLNames::miTag)
        || threadSafeMatch(tagName, MathMLNames::moTag)
        || threadSafeMatch(tagName, MathMLNames::mnTag)
        || threadSafeMatch(tagName, MathMLNames::msTag)
        || threadSafeMatch(tagName, MathMLNames::mtextTag);
#endif
}

HTMLTreeBuilderSimulator::HTMLTreeBuilderSimulator(const HTMLParserOptions& options)
    : m_options(options)
{
    m_namespaceStack.append(HTML);
}

HTMLTreeBuilderSimulator::State HTMLTreeBuilderSimulator::stateFor(HTMLTreeBuilder* treeBuilder)
{
    ASSERT(isMainThread());
    State namespaceStack;
    for (HTMLElementStack::ElementRecord* record = treeBuilder->openElements()->topRecord(); record; record = record->next()) {
        Namespace currentNamespace = HTML;
        ASSERT(false); // BKTODO:
#if 0
        if (record->namespaceURI() == SVGNames::svgNamespaceURI)
            currentNamespace = SVG;
        else if (record->namespaceURI() == MathMLNames::mathmlNamespaceURI)
            currentNamespace = MathML;
#endif

        if (namespaceStack.isEmpty() || namespaceStack.last() != currentNamespace)
            namespaceStack.append(currentNamespace);
    }
    namespaceStack.reverse();
    return namespaceStack;
}

HTMLTreeBuilderSimulator::SimulatedToken HTMLTreeBuilderSimulator::simulate(const CompactHTMLToken& token, HTMLTokenizer* tokenizer)
{
    SimulatedToken simulatedToken = OtherToken;

    if (token.type() == HTMLToken::StartTag) {
        const String& tagName = token.data();
        ASSERT(false); // BKTODO:
#if 0
        if (threadSafeMatch(tagName, SVGNames::svgTag))
            m_namespaceStack.append(SVG);
        if (threadSafeMatch(tagName, MathMLNames::mathTag))
            m_namespaceStack.append(MathML);
#endif
        if (inForeignContent() && tokenExitsForeignContent(token))
            m_namespaceStack.removeLast();
        if ((m_namespaceStack.last() == SVG && tokenExitsSVG(token))
            || (m_namespaceStack.last() == MathML && tokenExitsMath(token)))
            m_namespaceStack.append(HTML);
        if (!inForeignContent()) {
            // FIXME: This is just a copy of Tokenizer::updateStateFor which uses threadSafeMatches.
            if (threadSafeMatch(tagName, textareaTag) || threadSafeMatch(tagName, titleTag)) {
                tokenizer->setState(HTMLTokenizer::RCDATAState);
            } else if (threadSafeMatch(tagName, plaintextTag)) {
                tokenizer->setState(HTMLTokenizer::PLAINTEXTState);
            } else if (threadSafeMatch(tagName, scriptTag)) {
                tokenizer->setState(HTMLTokenizer::ScriptDataState);
                simulatedToken = ScriptStart;
            } else if (threadSafeMatch(tagName, styleTag)
                // BKTODO: || threadSafeMatch(tagName, iframeTag)
                || threadSafeMatch(tagName, xmpTag)
                // BKTODO: || (threadSafeMatch(tagName, noembedTag) && m_options.pluginsEnabled)
                // BKTODO: || threadSafeMatch(tagName, noframesTag)
#if 0 // BKTODO:
                || (threadSafeMatch(tagName, noscriptTag) && m_options.scriptEnabled)) {
#else
                )
            {
#endif
                tokenizer->setState(HTMLTokenizer::RAWTEXTState);
            }
        }
    }

    if (token.type() == HTMLToken::EndTag) {
        const String& tagName = token.data();
        ASSERT(false); // BKTODO:
#if 0
        if ((m_namespaceStack.last() == SVG && threadSafeMatch(tagName, SVGNames::svgTag))
            || (m_namespaceStack.last() == MathML && threadSafeMatch(tagName, MathMLNames::mathTag))
            || (m_namespaceStack.contains(SVG) && m_namespaceStack.last() == HTML && tokenExitsSVG(token))
            || (m_namespaceStack.contains(MathML) && m_namespaceStack.last() == HTML && tokenExitsMath(token)))
            m_namespaceStack.removeLast();
        if (threadSafeMatch(tagName, scriptTag)) {
            if (!inForeignContent())
                tokenizer->setState(HTMLTokenizer::DataState);
            return ScriptEnd;
        }
#endif
    }

    // FIXME: Also setForceNullCharacterReplacement when in text mode.
    tokenizer->setForceNullCharacterReplacement(inForeignContent());
    tokenizer->setShouldAllowCDATA(inForeignContent());
    return simulatedToken;
}

}
