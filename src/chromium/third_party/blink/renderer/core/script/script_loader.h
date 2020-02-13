// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_loader.h
// Description: ScriptLoader Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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

#ifndef BLINKIT_BLINK_SCRIPT_LOADER_H
#define BLINKIT_BLINK_SCRIPT_LOADER_H

#pragma once

#include "third_party/blink/renderer/core/script/pending_script.h"
#include "third_party/blink/renderer/core/script/script.h"
#include "third_party/blink/renderer/core/script/script_scheduling_type.h"
#include "third_party/blink/renderer/platform/bindings/name_client.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/text_position.h"

namespace blink {

class ScriptElementBase;

class ScriptLoader final : public GarbageCollectedFinalized<ScriptLoader>
                         // BKTODO:, public PendingScriptClient
                         , public NameClient
{
public:
    static std::unique_ptr<ScriptLoader> Create(ScriptElementBase *element, bool createdByParser, bool isEvaluated);
    ~ScriptLoader(void); // BKTODO: override;

    bool IsParserInserted(void) const { return m_parserInserted; }
    bool ReadyToBeParserExecuted(void) const { return m_readyToBeParserExecuted; }
    bool WillBeParserExecuted(void) const { return m_willBeParserExecuted; }
    bool WillExecuteWhenDocumentFinishedParsing(void) const { return m_willExecuteWhenDocumentFinishedParsing; }
    ScriptType GetScriptType(void) const { return m_scriptType; }

    enum LegacyTypeSupport {
        kDisallowLegacyTypeInTypeAttribute,
        kAllowLegacyTypeInTypeAttribute
    };
    static bool IsValidScriptTypeAndLanguage(const String &typeAttributeValue, const String &languageAttributeValue,
        LegacyTypeSupport supportLegacyTypes, ScriptType &outScriptType);

    static bool BlockForNoModule(ScriptType scriptType, bool nomodule);

    void HandleSourceAttribute(const String &sourceUrl);
    void DidNotifySubtreeInsertionsToDocument(void);
    void ChildrenChanged(void);

    std::shared_ptr<PendingScript> TakePendingScript(ScriptSchedulingType schedulingType);

    bool PrepareScript(const TextPosition &scriptStartPosition = TextPosition::MinimumPosition(),
        LegacyTypeSupport supportLegacyTypes = kDisallowLegacyTypeInTypeAttribute);
private:
    ScriptLoader(ScriptElementBase *element, bool parserInserted, bool alreadyStarted);

    bool IgnoresLoadRequest(void) const;
    bool IsScriptForEventSupported(void) const;

    const char* NameInHeapSnapshot(void) const override;

    Member<ScriptElementBase> m_element;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#already-started">
    // ... Initially, script elements must have this flag unset ...</spec>
    bool m_alreadyStarted = false;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#parser-inserted">
    // ... Initially, script elements must have this flag unset. ...</spec>
    bool m_parserInserted = false;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#non-blocking">
    // ... Initially, script elements must have this flag set. ...</spec>
    bool m_nonBlocking = true;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#ready-to-be-parser-executed">
    // ... Initially, script elements must have this flag unset ...</spec>
    bool m_readyToBeParserExecuted = false;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#concept-script-type">
    // ... It is determined when the script is prepared, ...</spec>
    ScriptType m_scriptType = ScriptType::kClassic;

    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#concept-script-external">
    // ... It is determined when the script is prepared, ...</spec>
    bool m_isExternalScript = false;

    // Same as "The parser will handle executing the script."
    bool m_willBeParserExecuted = false;

    bool m_willExecuteWhenDocumentFinishedParsing = false;

    std::unique_ptr<PendingScript> m_preparedPendingScript;
};

}  // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_LOADER_H
