#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_loader.h
// Description: ScriptLoader Class
//      Author: Ziming Li
//     Created: 2021-11-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef ScriptLoader_h
#define ScriptLoader_h

#include "blinkit/blink/renderer/core/dom/PendingScript.h"
#include "blinkit/blink/renderer/core/fetch/FetchRequest.h"
#include "blinkit/blink/renderer/core/fetch/ResourceClient.h"
#include "blinkit/blink/renderer/core/fetch/ResourcePtr.h"
#include "blinkit/blink/renderer/core/fetch/ScriptResource.h"
#include "blinkit/blink/renderer/wtf/text/TextPosition.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class Element;
class ScriptLoaderClient;
class ScriptSourceCode;
class LocalFrame;

class ScriptLoader : private ScriptResourceClient
{
public:
    static std::unique_ptr<ScriptLoader> create(Element* element, bool createdByParser, bool isEvaluated)
    {
        return zed::wrap_unique(new ScriptLoader(element, createdByParser, isEvaluated));
    }

    ~ScriptLoader() override;

    Element* element() const { return m_element; }

    enum LegacyTypeSupport { DisallowLegacyTypeInTypeAttribute, AllowLegacyTypeInTypeAttribute };
    bool prepareScript(const TextPosition& scriptStartPosition = TextPosition::minimumPosition(), LegacyTypeSupport = DisallowLegacyTypeInTypeAttribute);

    String scriptCharset() const { return m_characterEncoding; }
    String scriptContent() const;
    // Returns false if and only if execution was blocked.
    bool executeScript(const ScriptSourceCode &);
    virtual void execute();

    // XML parser calls these
    void dispatchLoadEvent();
    void dispatchErrorEvent();
    bool isScriptTypeSupported(LegacyTypeSupport) const;

    bool haveFiredLoadEvent() const { return m_haveFiredLoad; }
    bool willBeParserExecuted() const { return m_willBeParserExecuted; }
    bool readyToBeParserExecuted() const { return m_readyToBeParserExecuted; }
    bool willExecuteWhenDocumentFinishedParsing() const { return m_willExecuteWhenDocumentFinishedParsing; }
    ResourcePtr<ScriptResource> resource() { return m_resource; }

    void setHaveFiredLoadEvent(bool haveFiredLoad) { m_haveFiredLoad = haveFiredLoad; }
    bool isParserInserted() const { return m_parserInserted; }
    bool alreadyStarted() const { return m_alreadyStarted; }
    bool forceAsync() const { return m_forceAsync; }

    // Helper functions used by our parent classes.
    void didNotifySubtreeInsertionsToDocument();
    void childrenChanged();
    void handleSourceAttribute(const String& sourceUrl);
    void handleAsyncAttribute();

    virtual bool isReady() const { return m_pendingScript.isReady(); }

    // Clears the connection to the PendingScript (and Element and Resource).
    void detach();

protected:
    ScriptLoader(Element*, bool createdByParser, bool isEvaluated);

private:
    bool ignoresLoadRequest() const;
#ifdef BLINKIT_CRAWLER_ENABLED
    bool isScriptForEventSupported() const;
#endif
    void logScriptMimetype(ScriptResource*, LocalFrame*, String);

    bool fetchScript(const String& sourceUrl, FetchRequest::DeferOption);

    ScriptLoaderClient* client() const;

    // ResourceClient
    void notifyFinished(Resource*) override;
    String debugName() const override { return "ScriptLoader"; }

    Element *m_element;
    ResourcePtr<ScriptResource> m_resource;
    WTF::OrdinalNumber m_startLineNumber;
    String m_characterEncoding;
    String m_fallbackCharacterEncoding;

    bool m_parserInserted : 1;
    bool m_isExternalScript : 1;
    bool m_alreadyStarted : 1;
    bool m_haveFiredLoad : 1;
    bool m_willBeParserExecuted : 1; // Same as "The parser will handle executing the script."
    bool m_readyToBeParserExecuted : 1;
    bool m_willExecuteInOrder : 1;
    bool m_willExecuteWhenDocumentFinishedParsing : 1;
    bool m_forceAsync : 1;

    PendingScript m_pendingScript;
};

ScriptLoader* toScriptLoaderIfPossible(Element*);

} // namespace blink

#endif // ScriptLoader_h
