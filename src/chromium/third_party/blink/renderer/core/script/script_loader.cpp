// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_loader.cpp
// Description: ScriptLoader Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights
 * reserved.
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
 */

#include "script_loader.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/script/classic_pending_script.h"
#include "third_party/blink/renderer/core/script/script_element_base.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"

using namespace BlinKit;

namespace blink {

ScriptLoader::ScriptLoader(ScriptElementBase *element, bool parserInserted, bool alreadyStarted)
    : m_element(element)
{
    // <spec
    // href="https://html.spec.whatwg.org/multipage/scripting.html#already-started">
    // ... The cloning steps for script elements must set the "already started"
    // flag on the copy if it is set on the element being cloned.</spec>
    //
    // TODO(hiroshige): Cloning is implemented together with
    // {HTML,SVG}ScriptElement::cloneElementWithoutAttributesAndChildren().
    // Clean up these later.
    if (alreadyStarted)
        m_alreadyStarted = true;

    if (parserInserted)
    {
        // <spec
        // href="https://html.spec.whatwg.org/multipage/scripting.html#parser-inserted">
        // ... It is set by the HTML parser and the XML parser on script elements
        // they insert ...</spec>
        m_parserInserted = true;

        // <spec
        // href="https://html.spec.whatwg.org/multipage/scripting.html#non-blocking">
        // ... It is unset by the HTML parser and the XML parser on script elements
        // they insert. ...</spec>
        m_nonBlocking = false;
    }
}

ScriptLoader::~ScriptLoader(void) = default;

bool ScriptLoader::BlockForNoModule(ScriptType scriptType, bool nomodule)
{
    return nomodule && ScriptType::kClassic == scriptType;
}

void ScriptLoader::ChildrenChanged(void)
{
    if (!m_parserInserted && m_element->IsConnected())
        PrepareScript();  // FIXME: Provide a real starting line number here.
}

std::unique_ptr<ScriptLoader> ScriptLoader::Create(ScriptElementBase *element, bool createdByParser, bool isEvaluated)
{
    return base::WrapUnique(new ScriptLoader(element, createdByParser, isEvaluated));
}

void ScriptLoader::DidNotifySubtreeInsertionsToDocument(void)
{
    if (!m_parserInserted)
        PrepareScript();  // FIXME: Provide a real starting line number here.
}

// https://html.spec.whatwg.org/multipage/webappapis.html#fetch-a-classic-script
void ScriptLoader::FetchClassicScript(const BkURL &url, Document &elementDocument, const WTF::TextEncoding &encoding)
{
    std::shared_ptr<ClassicPendingScript> pendingScript = ClassicPendingScript::Fetch(url, elementDocument,
        encoding, m_element);
    ResourceClient *resourceClient = pendingScript.get();
    m_preparedPendingScript = pendingScript;
    m_resourceKeepAlive = resourceClient->GetResource();
}

void ScriptLoader::HandleSourceAttribute(const String &sourceUrl)
{
    if (IgnoresLoadRequest() || sourceUrl.IsEmpty())
        return;

    PrepareScript();  // FIXME: Provide a real starting line number here.
}

bool ScriptLoader::IgnoresLoadRequest(void) const
{
    return m_alreadyStarted || m_isExternalScript || m_parserInserted || !m_element->IsConnected();
}

// https://html.spec.whatwg.org/multipage/scripting.html#prepare-a-script
bool ScriptLoader::IsScriptForEventSupported(void) const
{
    // <spec step="14.1">Let for be the value of the for attribute.</spec>
    String eventAttribute = m_element->EventAttributeValue();
    // <spec step="14.2">Let event be the value of the event attribute.</spec>
    String forAttribute = m_element->ForAttributeValue();

    // <spec step="14">If the script element has an event attribute and a for
    // attribute, and the script's type is "classic", then:</spec>
    if (GetScriptType() != ScriptType::kClassic || eventAttribute.IsNull() || forAttribute.IsNull())
        return true;

    // <spec step="14.3">Strip leading and trailing ASCII whitespace from event
    // and for.</spec>
    forAttribute = forAttribute.StripWhiteSpace();
    // <spec step="14.4">If for is not an ASCII case-insensitive match for the
    // string "window", then return. The script is not executed.</spec>
    if (!DeprecatedEqualIgnoringCase(forAttribute, "window"))
        return false;
    eventAttribute = eventAttribute.StripWhiteSpace();
    // <spec step="14.5">If event is not an ASCII case-insensitive match for
    // either the string "onload" or the string "onload()", then return. The
    // script is not executed.</spec>
    return DeprecatedEqualIgnoringCase(eventAttribute, "onload")
        || DeprecatedEqualIgnoringCase(eventAttribute, "onload()");
}

static bool IsValidClassicScriptTypeAndLanguage(
    const String &type, const String &language,
    ScriptLoader::LegacyTypeSupport supportLegacyTypes)
{
    // FIXME: isLegacySupportedJavaScriptLanguage() is not valid HTML5. It is used
    // here to maintain backwards compatibility with existing layout tests. The
    // specific violations are:
    // - Allowing type=javascript. type= should only support MIME types, such as
    //   text/javascript.
    // - Allowing a different set of languages for language= and type=. language=
    //   supports Javascript 1.1 and 1.4-1.6, but type= does not.
    if (type.IsEmpty())
    {
        return language.IsEmpty() // assume text/javascript.
            || MIMETypeRegistry::IsSupportedJavaScriptMIMEType("text/" + language)
            || MIMETypeRegistry::IsLegacySupportedJavaScriptLanguage(language);
    }

    if (MIMETypeRegistry::IsSupportedJavaScriptMIMEType(type.StripWhiteSpace()))
        return true;

    if (ScriptLoader::kAllowLegacyTypeInTypeAttribute == supportLegacyTypes)
    {
        if (MIMETypeRegistry::IsLegacySupportedJavaScriptLanguage(type))
            return true;
    }

    return false;
}

bool ScriptLoader::IsValidScriptTypeAndLanguage(
    const String &typeAttributeValue, const String &languageAttributeValue,
    LegacyTypeSupport supportLegacyTypes,
    ScriptType &outScriptType)
{
    if (IsValidClassicScriptTypeAndLanguage(typeAttributeValue, languageAttributeValue, supportLegacyTypes))
    {
        // <spec step="7">... If the script block's type string is a JavaScript MIME
        // type essence match, the script's type is "classic". ...</spec>
        //
        // TODO(hiroshige): Annotate and/or cleanup this step.
        outScriptType = ScriptType::kClassic;
        return true;
    }

    if (typeAttributeValue == "module")
    {
        // <spec step="7">... If the script block's type string is an ASCII
        // case-insensitive match for the string "module", the script's type is
        // "module". ...</spec>
        outScriptType = ScriptType::kModule;
        return true;
    }

    // <spec step="7">... If neither of the above conditions are true, then
    // return. No script is executed.</spec>
    return false;
}

const char* ScriptLoader::NameInHeapSnapshot(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

bool ScriptLoader::PrepareScript(const TextPosition &scriptStartPosition, LegacyTypeSupport supportLegacyTypes)
{
    // <spec step="1">If the script element is marked as having "already started",
    // then return. The script is not executed.</spec>
    if (m_alreadyStarted)
        return false;

    // <spec step="2">If the element has its "parser-inserted" flag set, then set
    // was-parser-inserted to true and unset the element's "parser-inserted" flag.
    // Otherwise, set was-parser-inserted to false.</spec>
    bool wasParserInserted;
    if (m_parserInserted)
    {
        wasParserInserted = true;
        m_parserInserted = false;
    }
    else
    {
        wasParserInserted = false;
    }

    // <spec step="3">If was-parser-inserted is true and the element does not have
    // an async attribute, then set the element's "non-blocking" flag to
    // true.</spec>
    if (wasParserInserted && !m_element->AsyncAttributeValue())
        m_nonBlocking = true;

    // <spec step="4">Let source text be the element's child text content.</spec>
    const String source_text = m_element->TextFromChildren();

    // <spec step="5">If the element has no src attribute, and source text is the
    // empty string, then return. The script is not executed.</spec>
    if (!m_element->HasSourceAttribute() && source_text.IsEmpty())
        return false;

    // <spec step="6">If the element is not connected, then return. The script is
    // not executed.</spec>
    if (!m_element->IsConnected())
        return false;

    // <spec step="7">... Determine the script's type as follows: ...</spec>
    //
    // |script_type_| is set here.

    if (!IsValidScriptTypeAndLanguage(m_element->TypeAttributeValue(), m_element->LanguageAttributeValue(),
        supportLegacyTypes, m_scriptType))
    {
        return false;
    }

    // <spec step="8">If was-parser-inserted is true, then flag the element as
    // "parser-inserted" again, and set the element's "non-blocking" flag to
    // false.</spec>
    if (wasParserInserted)
    {
        m_parserInserted = true;
        m_nonBlocking = false;
    }

    // <spec step="9">Set the element's "already started" flag.</spec>
    m_alreadyStarted = true;

    // <spec step="10">If the element is flagged as "parser-inserted", but the
    // element's node document is not the Document of the parser that created the
    // element, then return.</spec>
    //
    // FIXME: If script is parser inserted, verify it's still in the original
    // document.

    // <spec step="11">If scripting is disabled for the script element, then
    // return. The script is not executed.</spec>
    //
    // <spec
    // href="https://html.spec.whatwg.org/multipage/webappapis.html#concept-n-noscript">
    // Scripting is disabled for a node if [the node's node document has no
    // browsing context], or if scripting is disabled in that browsing context.
    // </spec>
    Document &elementDocument = m_element->GetDocument();
    // TODO(timothygu): Investigate if we could switch from ExecutingFrame() to
    // ExecutingWindow().
    if (nullptr == elementDocument.ExecutingFrame())
        return false;

    Document *contextDocument = elementDocument.ContextDocument();
    if (nullptr == contextDocument || nullptr == contextDocument->ExecutingFrame())
        return false;
    if (!contextDocument->CanExecuteScripts(kAboutToExecuteScript))
        return false;

    // <spec step="12">If the script element has a nomodule content attribute and
    // the script's type is "classic", then return. The script is not
    // executed.</spec>
    if (BlockForNoModule(m_scriptType, m_element->NomoduleAttributeValue()))
        return false;

    // 13.
    if (!IsScriptForEventSupported())
        return false;

    // 14. is handled below.

    // <spec step="21">Let parser metadata be "parser-inserted" if the script
    // element has been flagged as "parser-inserted", and "not-parser-inserted"
    // otherwise.</spec>
    ParserDisposition parserState = IsParserInserted() ? kParserInserted : kNotParserInserted;

    ASSERT(!m_preparedPendingScript);

    // TODO(csharrison): This logic only works if the tokenizer/parser was not
    // blocked waiting for scripts when the element was inserted. This usually
    // fails for instance, on second document.write if a script writes twice
    // in a row. To fix this, the parser might have to keep track of raw
    // string position.
    //
    // Also PendingScript's contructor has the same code.
    const bool isInDocumentWrite = elementDocument.IsInDocumentWrite();

    // Reset line numbering for nested writes.
    TextPosition position = isInDocumentWrite ? TextPosition() : scriptStartPosition;

#if 0 // BKTODO: Check if necessary
    // <spec step="22">Let options be a script fetch options whose cryptographic
    // nonce is cryptographic nonce, integrity metadata is integrity metadata,
    // parser metadata is parser metadata, credentials mode is module script
    // credentials mode, and referrer policy is referrer policy.</spec>
    ScriptFetchOptions options(nonce, integrity_metadata, integrity_attr,
        parser_state, credentials_mode, referrer_policy);

    // <spec step="23">Let settings object be the element's node document's Window
    // object's environment settings object.</spec>
    //
    // Note: We use |element_document| as "settings object" in the steps below.
    auto* settings_object =
        element_document.CreateFetchClientSettingsObjectSnapshot();
#endif

    // <spec step="24">If the element has a src content attribute, then:</spec>
    if (m_element->HasSourceAttribute())
    {
        // <spec step="24.1">Let src be the value of the element's src
        // attribute.</spec>
        String src = StripLeadingAndTrailingHTMLSpaces(m_element->SourceAttributeValue());

        // <spec step="24.2">If src is the empty string, queue a task to fire an
        // event named error at the element, and return.</spec>
        if (src.IsEmpty())
        {
            ASSERT(false); // BKTODO:
#if 0
            element_document.GetTaskRunner(TaskType::kDOMManipulation)
                ->PostTask(FROM_HERE,
                    WTF::Bind(&ScriptElementBase::DispatchErrorEvent,
                        WrapPersistent(element_.Get())));
#endif
            return false;
        }

        // <spec step="24.3">Set the element's from an external file flag.</spec>
        m_isExternalScript = true;

        // <spec step="24.4">Parse src relative to the element's node
        // document.</spec>
        BkURL url = elementDocument.CompleteURL(src);

        // <spec step="24.5">If the previous step failed, queue a task to fire an
        // event named error at the element, and return. Otherwise, let url be the
        // resulting URL record.</spec>
        if (!url.IsValid())
        {
            ASSERT(false); // BKTODO:
#if 0
            element_document.GetTaskRunner(TaskType::kDOMManipulation)
                ->PostTask(FROM_HERE,
                    WTF::Bind(&ScriptElementBase::DispatchErrorEvent,
                        WrapPersistent(element_.Get())));
#endif
            return false;
        }

        // <spec step="24.6">Switch on the script's type:</spec>
        if (GetScriptType() == ScriptType::kClassic)
        {
            // - "classic":

            // <spec step="15">If the script element has a charset attribute, then let
            // encoding be the result of getting an encoding from the value of the
            // charset attribute. If the script element does not have a charset
            // attribute, or if getting an encoding failed, let encoding be the same
            // as the encoding of the script element's node document.</spec>
            //
            // TODO(hiroshige): Should we handle failure in getting an encoding?
            WTF::TextEncoding encoding;
            String charset = m_element->CharsetAttributeValue();
            if (!charset.IsEmpty())
                encoding = WTF::TextEncoding(charset);
            else
                encoding = elementDocument.Encoding();

            // <spec step="24.6.A">"classic"
            //
            // Fetch a classic script given url, settings object, options, classic
            // script CORS setting, and encoding.</spec>
            FetchClassicScript(url, elementDocument, encoding);
        }
        else
        {
            ASSERT(false); // BKTODO:
#if 0
            // - "module":

            // Step 15 is skipped because they are not used in module
            // scripts.

            // <spec step="24.6.B">"module"
            //
            // Fetch a module script graph given url, settings object, "script", and
            // options.</spec>
            Modulator* modulator = Modulator::From(
                ToScriptStateForMainWorld(context_document->GetFrame()));
            FetchModuleScriptTree(url, settings_object, modulator, options);
#endif
        }
        // <spec step="24.6">When the chosen algorithm asynchronously completes, set
        // the script's script to the result. At that time, the script is ready.
        // ...</spec>
        //
        // When the script is ready, PendingScriptClient::pendingScriptFinished()
        // is used as the notification, and the action to take when
        // the script is ready is specified later, in
        // - ScriptLoader::PrepareScript(), or
        // - HTMLParserScriptRunner,
        // depending on the conditions in Step 25 of "prepare a script".
    }

    // <spec step="25">If the element does not have a src content attribute, run
    // these substeps:</spec>
    if (!m_element->HasSourceAttribute())
    {
        // <spec step="25.1">Let src be the value of the element's src
        // attribute.</spec>
        //
        // This step is done later as ScriptElementBase::TextFromChildren():
        // - in ScriptLoader::PrepareScript() (Step 26, 6th Clause),
        // - in HTMLParserScriptRunner::ProcessScriptElementInternal()
        //   (Duplicated code of Step 26, 6th Clause),
        // - in XMLDocumentParser::EndElementNs() (Step 26, 5th Clause), or
        // - in PendingScript::GetSource() (Indirectly used via
        //   HTMLParserScriptRunner::ProcessScriptElementInternal(),
        //   Step 26, 5th Clause).

        // <spec step="25.1">Let base URL be the script element's node document's
        // document base URL.</spec>
        BkURL baseUrl = elementDocument.BaseURL();

        // <spec step="25.2">Switch on the script's type:</spec>
        switch (GetScriptType())
        {
            // Step 25.2.A. "classic" [spec text]
            case ScriptType::kClassic:
            {
                // <spec step="25.2.A.1">Let script be the result of creating a classic
                // script using source text, settings object, base URL, and
                // options.</spec>
                ScriptSourceLocationType scriptLocationType = ScriptSourceLocationType::kInline;
                if (!m_parserInserted)
                    scriptLocationType = ScriptSourceLocationType::kInlineInsideGeneratedElement;
                else if (isInDocumentWrite)
                    scriptLocationType = ScriptSourceLocationType::kInlineInsideDocumentWrite;

                m_preparedPendingScript = ClassicPendingScript::CreateInline(m_element, position, scriptLocationType);

                // <spec step="25.2.A.2">Set the script's script to script.</spec>
                //
                // <spec step="25.2.A.3">The script is ready.</spec>
                //
                // Implemented by ClassicPendingScript.
                break;
            }
            // Step 25.2.B. "module" [spec text]
            case ScriptType::kModule:
            {
                ASSERT(false); // BKTODO:
#if 0
                // <spec step="25.2.B.1">Let script be the result of creating a module
                // script using source text, settings object, base URL, and
                // options.</spec>
                const KURL& source_url = element_document.Url();
                Modulator* modulator = Modulator::From(
                    ToScriptStateForMainWorld(context_document->GetFrame()));
                ModuleScript* module_script = ModuleScript::Create(
                    ParkableString(element_->TextFromChildren().Impl()), modulator,
                    source_url, base_url, options, kSharableCrossOrigin, position);
                // <spec step="25.2.B.2">If this returns null, set the script's script
                // to null and return; the script is ready.</spec>
                if (!module_script)
                    return false;

                // <spec step="25.2.B.3">Fetch the descendants of and instantiate
                // script, given the destination "script". When this asynchronously
                // completes, set the script's script to the result. At that time, the
                // script is ready.</spec>
                auto* module_tree_client = ModulePendingScriptTreeClient::Create();
                modulator->FetchDescendantsForInlineScript(
                    module_script, settings_object, mojom::RequestContextType::SCRIPT,
                    module_tree_client);
                prepared_pending_script_ = ModulePendingScript::Create(
                    element_, module_tree_client, is_external_script_);
#endif
                break;
            }
        }
    }

    ASSERT(m_preparedPendingScript);

#if 0 // BKTODO: defer
    // <spec step="26">Then, follow the first of the following options that
    // describes the situation:</spec>

    // Three flags are used to instruct the caller of prepareScript() to execute
    // a part of Step 25, when |m_willBeParserExecuted| is true:
    // - |m_willBeParserExecuted|
    // - |m_willExecuteWhenDocumentFinishedParsing|
    // - |m_readyToBeParserExecuted|
    // TODO(hiroshige): Clean up the dependency.

    // <spec step="26.A">If the script's type is "classic", and the element has a
    // src attribute, and the element has a defer attribute, and the element has
    // been flagged as "parser-inserted", and the element does not have an async
    // attribute
    //
    // If the script's type is "module", and the element has been flagged as
    // "parser-inserted", and the element does not have an async attribute
    // ...</spec>
    if ((GetScriptType() == ScriptType::kClassic &&
        element_->HasSourceAttribute() && element_->DeferAttributeValue() &&
        parser_inserted_ && !element_->AsyncAttributeValue()) ||
        (GetScriptType() == ScriptType::kModule && parser_inserted_ &&
            !element_->AsyncAttributeValue())) {
        // This clause is implemented by the caller-side of prepareScript():
        // - HTMLParserScriptRunner::requestDeferredScript(), and
        // - TODO(hiroshige): Investigate XMLDocumentParser::endElementNs()
        will_execute_when_document_finished_parsing_ = true;
        will_be_parser_executed_ = true;

        return true;
    }
#endif

    // <spec step="26.B">If the script's type is "classic", and the element has a
    // src attribute, and the element has been flagged as "parser-inserted", and
    // the element does not have an async attribute ...</spec>
    if (GetScriptType() == ScriptType::kClassic && m_element->HasSourceAttribute() && m_parserInserted && !m_element->AsyncAttributeValue())
    {
        // This clause is implemented by the caller-side of prepareScript():
        // - HTMLParserScriptRunner::requestParsingBlockingScript()
        // - TODO(hiroshige): Investigate XMLDocumentParser::endElementNs()
        m_willBeParserExecuted = true;

        return true;
    }

    // <spec step="26.C">If the script's type is "classic", and the element has a
    // src attribute, and the element does not have an async attribute, and the
    // element does not have the "non-blocking" flag set
    //
    // If the script's type is "module", and the element does not have an async
    // attribute, and the element does not have the "non-blocking" flag set
    // ...</spec>
    if ((GetScriptType() == ScriptType::kClassic && m_element->HasSourceAttribute() && !m_element->AsyncAttributeValue() && !m_nonBlocking)
        || (GetScriptType() == ScriptType::kModule && !m_element->AsyncAttributeValue() && !m_nonBlocking))
    {
        ASSERT(false); // BKTODO:
#if 0
        // <spec step="26.C">... Add the element to the end of the list of scripts
        // that will execute in order as soon as possible associated with the node
        // document of the script element at the time the prepare a script algorithm
        // started. ...</spec>
        pending_script_ = TakePendingScript(ScriptSchedulingType::kInOrder);
        // TODO(hiroshige): Here |contextDocument| is used as "node document"
        // while Step 14 uses |elementDocument| as "node document". Fix this.
        context_document->GetScriptRunner()->QueueScriptForExecution(
            pending_script_);
        // Note that watchForLoad can immediately call pendingScriptFinished.
        pending_script_->WatchForLoad(this);
        // The part "When the script is ready..." is implemented in
        // ScriptRunner::notifyScriptReady().
        // TODO(hiroshige): Annotate it.
#endif

        return true;
    }

    // <spec step="26.D">If the script's type is "classic", and the element has a
    // src attribute
    //
    // If the script's type is "module" ...</spec>
    if ((GetScriptType() == ScriptType::kClassic && m_element->HasSourceAttribute())
        || GetScriptType() == ScriptType::kModule)
    {
        ASSERT(false); // BKTODO:
#if 0
        // <spec step="26.D">... The element must be added to the set of scripts
        // that will execute as soon as possible of the node document of the script
        // element at the time the prepare a script algorithm started. When the
        // script is ready, execute the script block and then remove the element
        // from the set of scripts that will execute as soon as possible.</spec>
        pending_script_ = TakePendingScript(ScriptSchedulingType::kAsync);
        // TODO(hiroshige): Here |contextDocument| is used as "node document"
        // while Step 14 uses |elementDocument| as "node document". Fix this.
        context_document->GetScriptRunner()->QueueScriptForExecution(
            pending_script_);
        // Note that watchForLoad can immediately call pendingScriptFinished.
        pending_script_->WatchForLoad(this);
        // The part "When the script is ready..." is implemented in
        // ScriptRunner::notifyScriptReady().
        // TODO(hiroshige): Annotate it.
#endif

        return true;
    }

    // The following clauses are executed only if the script's type is "classic"
    // and the element doesn't have a src attribute.
    ASSERT(GetScriptType() == ScriptType::kClassic);
    ASSERT(!m_isExternalScript);

    // <spec step="26.E">If the element does not have a src attribute, and the
    // element has been flagged as "parser-inserted", and either the parser that
    // created the script is an XML parser or it's an HTML parser whose script
    // nesting level is not greater than one, and the Document of the HTML parser
    // or XML parser that created the script element has a style sheet that is
    // blocking scripts ...</spec>
    //
    // The last part "... has a style sheet that is blocking scripts"
    // is implemented in Document::isScriptExecutionReady().
    // Part of the condition check is done in
    // HTMLParserScriptRunner::processScriptElementInternal().
    // TODO(hiroshige): Clean up the split condition check.
    if (!m_element->HasSourceAttribute() && m_parserInserted && !elementDocument.IsScriptExecutionReady())
    {
        // The former part of this clause is
        // implemented by the caller-side of prepareScript():
        // - HTMLParserScriptRunner::requestParsingBlockingScript()
        // - TODO(hiroshige): Investigate XMLDocumentParser::endElementNs()
        m_willBeParserExecuted = true;
        // <spec step="26.E">... Set the element's "ready to be parser-executed"
        // flag. ...</spec>
        m_readyToBeParserExecuted = true;

        return true;
    }

    // <spec step="26.F">Otherwise
    //
    // Immediately execute the script block, even if other scripts are already
    // executing.</spec>
    //
    // Note: this block is also duplicated in
    // HTMLParserScriptRunner::processScriptElementInternal().
    // TODO(hiroshige): Merge the duplicated code.
    BkURL scriptUrl = (!isInDocumentWrite && m_parserInserted)
        ? elementDocument.Url()
        : BkURL();
    TakePendingScript(ScriptSchedulingType::kImmediate)->ExecuteScriptBlock(scriptUrl);
    return true;
}

std::shared_ptr<PendingScript> ScriptLoader::TakePendingScript(ScriptSchedulingType schedulingType)
{
    ASSERT(m_preparedPendingScript);

    switch (schedulingType)
    {
        case ScriptSchedulingType::kAsync:
        case ScriptSchedulingType::kInOrder:
            // As ClassicPendingScript keeps a reference to ScriptResource,
            // the ScriptResource is anyway kept alive until evaluation,
            // and can be garbage-collected after that (together with
            // ClassicPendingScript).
            ASSERT(false); // BKTODO: m_resourceKeepAlive = nullptr;
            break;
        default:
            // ScriptResource is kept alive by resource_keep_alive_
            // until ScriptLoader is garbage collected.
            break;
    }

    std::shared_ptr<PendingScript> pendingScript = std::move(m_preparedPendingScript);
    pendingScript->SetSchedulingType(schedulingType);
    return pendingScript;
}

}  // namespace blink
