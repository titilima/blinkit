// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FontFaceSet.h
// Description: FontFaceSet Class
//      Author: Ziming Li
//     Created: 2019-02-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "core/css/FontFaceSet.h"

#include "core/css/CSSFontSelector.h"
#include "core/css/CSSSegmentedFontFace.h"
#include "core/css/FontFaceCache.h"
#include "core/css/FontFaceSetLoadEvent.h"
#include "core/css/StylePropertySet.h"
#include "core/css/parser/CSSParser.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/Document.h"
#include "core/dom/StyleEngine.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/style/StyleInheritedData.h"
#include "public/platform/Platform.h"

namespace blink {

static const int defaultFontSize = 10;
static const char defaultFontFamily[] = "sans-serif";

class LoadFontPromiseResolver final : public FontFace::LoadFontCallback {
public:
    static PassRefPtrWillBeRawPtr<LoadFontPromiseResolver> create(FontFaceArray faces, ScriptState* scriptState)
    {
        return adoptRefWillBeNoop(new LoadFontPromiseResolver(faces, scriptState));
    }

    void loadFonts(ExecutionContext*);

    void notifyLoaded(FontFace*) override;
    void notifyError(FontFace*) override;

    DECLARE_VIRTUAL_TRACE();

private:
    LoadFontPromiseResolver(FontFaceArray faces, ScriptState* scriptState)
        : m_numLoading(faces.size())
        , m_errorOccured(false)
    {
        m_fontFaces.swap(faces);
    }

    WillBeHeapVector<RefPtrWillBeMember<FontFace>> m_fontFaces;
    int m_numLoading;
    bool m_errorOccured;
};

void LoadFontPromiseResolver::loadFonts(ExecutionContext* context)
{
    if (!m_numLoading) {
        assert(false); // BKTODO:
#if 0
        m_resolver->resolve(m_fontFaces);
#endif
        return;
    }

    for (size_t i = 0; i < m_fontFaces.size(); i++)
        m_fontFaces[i]->loadWithCallback(this, context);
}

void LoadFontPromiseResolver::notifyLoaded(FontFace* fontFace)
{
    m_numLoading--;
    if (m_numLoading || m_errorOccured)
        return;

    assert(false); // BKTODO:
#if 0
    m_resolver->resolve(m_fontFaces);
#endif
}

void LoadFontPromiseResolver::notifyError(FontFace* fontFace)
{
    m_numLoading--;
    if (!m_errorOccured) {
        m_errorOccured = true;
        assert(false); // BKTODO:
#if 0
        m_resolver->reject(fontFace->error());
#endif
    }
}

DEFINE_TRACE(LoadFontPromiseResolver)
{
    visitor->trace(m_fontFaces);
    LoadFontCallback::trace(visitor);
}

FontFaceSet::FontFaceSet(Document& document)
    : ActiveDOMObject(&document)
    , m_shouldFireLoadingEvent(false)
    , m_isLoading(false)
    , m_asyncRunner(AsyncMethodRunner<FontFaceSet>::create(this, &FontFaceSet::handlePendingEventsAndPromises))
{
    suspendIfNeeded();
}

FontFaceSet::~FontFaceSet()
{
#if !ENABLE(OILPAN)
    stop();
#endif
}

Document* FontFaceSet::document() const
{
    return toDocument(executionContext());
}

bool FontFaceSet::inActiveDocumentContext() const
{
    ExecutionContext* context = executionContext();
    return context && toDocument(context)->isActive();
}

void FontFaceSet::addFontFacesToFontFaceCache(FontFaceCache* fontFaceCache, CSSFontSelector* fontSelector)
{
    for (const auto& fontFace : m_nonCSSConnectedFaces)
        fontFaceCache->addFontFace(fontSelector, fontFace, false);
}

const AtomicString& FontFaceSet::interfaceName() const
{
    return EventTargetNames::FontFaceSet;
}

ExecutionContext* FontFaceSet::executionContext() const
{
    return ActiveDOMObject::executionContext();
}

AtomicString FontFaceSet::status() const
{
    DEFINE_STATIC_LOCAL(AtomicString, loading, ("loading", AtomicString::ConstructFromLiteral));
    DEFINE_STATIC_LOCAL(AtomicString, loaded, ("loaded", AtomicString::ConstructFromLiteral));
    return m_isLoading ? loading : loaded;
}

void FontFaceSet::handlePendingEventsAndPromisesSoon()
{
    // m_asyncRunner will be automatically stopped on destruction.
    m_asyncRunner->runAsync();
}

void FontFaceSet::didLayout()
{
    if (document()->frame()->isMainFrame() && m_loadingFonts.isEmpty())
        m_histogram.record();
    if (!shouldSignalReady())
        return;
    handlePendingEventsAndPromisesSoon();
}

bool FontFaceSet::shouldSignalReady() const
{
    if (!m_loadingFonts.isEmpty())
        return false;
    return m_isLoading;
}

void FontFaceSet::handlePendingEventsAndPromises()
{
    fireLoadingEvent();
    fireDoneEventIfPossible();
}

void FontFaceSet::fireLoadingEvent()
{
    if (m_shouldFireLoadingEvent) {
        m_shouldFireLoadingEvent = false;
        dispatchEvent(FontFaceSetLoadEvent::createForFontFaces(EventTypeNames::loading));
    }
}

void FontFaceSet::suspend()
{
    m_asyncRunner->suspend();
}

void FontFaceSet::resume()
{
    m_asyncRunner->resume();
}

void FontFaceSet::stop()
{
    m_asyncRunner->stop();
}

void FontFaceSet::beginFontLoading(FontFace* fontFace)
{
    m_histogram.incrementCount();
    addToLoadingFonts(fontFace);
}

void FontFaceSet::fontLoaded(FontFace* fontFace)
{
    m_histogram.updateStatus(fontFace);
    m_loadedFonts.append(fontFace);
    removeFromLoadingFonts(fontFace);
}

void FontFaceSet::loadError(FontFace* fontFace)
{
    m_histogram.updateStatus(fontFace);
    m_failedFonts.append(fontFace);
    removeFromLoadingFonts(fontFace);
}

void FontFaceSet::addToLoadingFonts(PassRefPtrWillBeRawPtr<FontFace> fontFace)
{
    if (!m_isLoading) {
        m_isLoading = true;
        m_shouldFireLoadingEvent = true;
        handlePendingEventsAndPromisesSoon();
    }
    m_loadingFonts.add(fontFace);
}

void FontFaceSet::removeFromLoadingFonts(PassRefPtrWillBeRawPtr<FontFace> fontFace)
{
    m_loadingFonts.remove(fontFace);
    if (m_loadingFonts.isEmpty())
        handlePendingEventsAndPromisesSoon();
}

const WillBeHeapListHashSet<RefPtrWillBeMember<FontFace>>& FontFaceSet::cssConnectedFontFaceList() const
{
    Document* d = document();
    d->ensureStyleResolver(); // Flush pending style changes.
    return d->styleEngine().fontSelector()->fontFaceCache()->cssConnectedFontFaces();
}

bool FontFaceSet::isCSSConnectedFontFace(FontFace* fontFace) const
{
    return cssConnectedFontFaceList().contains(fontFace);
}

size_t FontFaceSet::size() const
{
    if (!inActiveDocumentContext())
        return m_nonCSSConnectedFaces.size();
    return cssConnectedFontFaceList().size() + m_nonCSSConnectedFaces.size();
}

void FontFaceSet::fireDoneEventIfPossible()
{
    if (m_shouldFireLoadingEvent)
        return;
    if (!shouldSignalReady())
        return;

    // If the layout was invalidated in between when we thought layout
    // was updated and when we're ready to fire the event, just wait
    // until after the next layout before firing events.
    Document* d = document();
    if (!d->view() || d->view()->needsLayout())
        return;

    if (m_isLoading) {
        RefPtrWillBeRawPtr<FontFaceSetLoadEvent> doneEvent = nullptr;
        RefPtrWillBeRawPtr<FontFaceSetLoadEvent> errorEvent = nullptr;
        doneEvent = FontFaceSetLoadEvent::createForFontFaces(EventTypeNames::loadingdone, m_loadedFonts);
        m_loadedFonts.clear();
        if (!m_failedFonts.isEmpty()) {
            errorEvent = FontFaceSetLoadEvent::createForFontFaces(EventTypeNames::loadingerror, m_failedFonts);
            m_failedFonts.clear();
        }
        m_isLoading = false;
        dispatchEvent(doneEvent);
        if (errorEvent)
            dispatchEvent(errorEvent);
    }
}

bool FontFaceSet::check(const String& fontString, const String& text, ExceptionState& exceptionState)
{
    if (!inActiveDocumentContext())
        return false;

    Font font;
    if (!resolveFontStyle(fontString, font)) {
        exceptionState.throwDOMException(SyntaxError, "Could not resolve '" + fontString + "' as a font.");
        return false;
    }

    CSSFontSelector* fontSelector = document()->styleEngine().fontSelector();
    FontFaceCache* fontFaceCache = fontSelector->fontFaceCache();

    bool hasLoadedFaces = false;
    for (const FontFamily* f = &font.fontDescription().family(); f; f = f->next()) {
        CSSSegmentedFontFace* face = fontFaceCache->get(font.fontDescription(), f->family());
        if (face) {
            if (!face->checkFont(text))
                return false;
            hasLoadedFaces = true;
        }
    }
    if (hasLoadedFaces)
        return true;
    for (const FontFamily* f = &font.fontDescription().family(); f; f = f->next()) {
        if (fontSelector->isPlatformFontAvailable(font.fontDescription(), f->family()))
            return true;
    }
    return false;
}

bool FontFaceSet::resolveFontStyle(const String& fontString, Font& font)
{
    if (fontString.isEmpty())
        return false;

    // Interpret fontString in the same way as the 'font' attribute of CanvasRenderingContext2D.
    RefPtrWillBeRawPtr<MutableStylePropertySet> parsedStyle = MutableStylePropertySet::create(HTMLStandardMode);
    CSSParser::parseValue(parsedStyle.get(), CSSPropertyFont, fontString, true, 0);
    if (parsedStyle->isEmpty())
        return false;

    String fontValue = parsedStyle->getPropertyValue(CSSPropertyFont);
    if (fontValue == "inherit" || fontValue == "initial")
        return false;

    RefPtr<ComputedStyle> style = ComputedStyle::create();

    FontFamily fontFamily;
    fontFamily.setFamily(defaultFontFamily);

    FontDescription defaultFontDescription;
    defaultFontDescription.setFamily(fontFamily);
    defaultFontDescription.setSpecifiedSize(defaultFontSize);
    defaultFontDescription.setComputedSize(defaultFontSize);

    style->setFontDescription(defaultFontDescription);

    style->font().update(style->font().fontSelector());

    document()->ensureStyleResolver().computeFont(style.get(), *parsedStyle);

    font = style->font();
    font.update(document()->styleEngine().fontSelector());
    return true;
}

void FontFaceSet::FontLoadHistogram::updateStatus(FontFace* fontFace)
{
    if (m_status == Reported)
        return;
    if (fontFace->hadBlankText())
        m_status = HadBlankText;
    else if (m_status == NoWebFonts)
        m_status = DidNotHaveBlankText;
}

void FontFaceSet::FontLoadHistogram::record()
{
    if (!m_recorded) {
        m_recorded = true;
        Platform::current()->histogramCustomCounts("WebFont.WebFontsInPage", m_count, 1, 100, 50);
    }
    if (m_status == HadBlankText || m_status == DidNotHaveBlankText) {
        Platform::current()->histogramEnumeration("WebFont.HadBlankText", m_status == HadBlankText ? 1 : 0, 2);
        m_status = Reported;
    }
}

static const char* supplementName()
{
    return "FontFaceSet";
}

PassRefPtrWillBeRawPtr<FontFaceSet> FontFaceSet::from(Document& document)
{
    RefPtrWillBeRawPtr<FontFaceSet> fonts = static_cast<FontFaceSet*>(SupplementType::from(document, supplementName()));
    if (!fonts) {
        fonts = FontFaceSet::create(document);
        SupplementType::provideTo(document, supplementName(), fonts);
    }

    return fonts.release();
}

void FontFaceSet::didLayout(Document& document)
{
    if (FontFaceSet* fonts = static_cast<FontFaceSet*>(SupplementType::from(document, supplementName())))
        fonts->didLayout();
}

DEFINE_TRACE(FontFaceSet)
{
#if ENABLE(OILPAN)
    visitor->trace(m_ready);
    visitor->trace(m_loadingFonts);
    visitor->trace(m_loadedFonts);
    visitor->trace(m_failedFonts);
    visitor->trace(m_nonCSSConnectedFaces);
    visitor->trace(m_asyncRunner);
    HeapSupplement<Document>::trace(visitor);
#endif
    EventTargetWithInlineData::trace(visitor);
    ActiveDOMObject::trace(visitor);
}

} // namespace blink
