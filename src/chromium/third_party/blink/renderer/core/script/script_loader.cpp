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
#include "third_party/blink/renderer/core/script/script_element_base.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"

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

void ScriptLoader::HandleSourceAttribute(const String &sourceUrl)
{
    ASSERT(false); // BKTODO:
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

    ASSERT(false); // BKTODO:
#if 0
    // <spec step="12">If the script element has a nomodule content attribute and
    // the script's type is "classic", then return. The script is not
    // executed.</spec>
    if (BlockForNoModule(script_type_, element_->NomoduleAttributeValue()))
        return false;

    // 13.
    if (!IsScriptForEventSupported())
        return false;

    // This FeaturePolicy is still in the process of being added to the spec.
    if (ShouldBlockSyncScriptForFeaturePolicy(element_.Get(), GetScriptType(),
        parser_inserted_)) {
        element_document.AddConsoleMessage(ConsoleMessage::Create(
            kJSMessageSource, kErrorMessageLevel,
            "Synchronous script execution is disabled by Feature Policy"));
        return false;
    }

    // 14. is handled below.

    // <spec step="16">Let classic script CORS setting be the current state of the
    // element's crossorigin content attribute.</spec>
    CrossOriginAttributeValue cross_origin =
        GetCrossOriginAttributeValue(element_->CrossOriginAttributeValue());

    // <spec step="17">Let module script credentials mode be the module script
    // credentials mode for the element's crossorigin content attribute.</spec>
    network::mojom::FetchCredentialsMode credentials_mode =
        ModuleScriptCredentialsMode(cross_origin);

    // <spec step="18">Let cryptographic nonce be the element's
    // [[CryptographicNonce]] internal slot's value.</spec>
    String nonce = element_->GetNonceForElement();

    // <spec step="19">If the script element has an integrity attribute, then let
    // integrity metadata be that attribute's value. Otherwise, let integrity
    // metadata be the empty string.</spec>
    String integrity_attr = element_->IntegrityAttributeValue();
    IntegrityMetadataSet integrity_metadata;
    if (!integrity_attr.IsEmpty()) {
        SubresourceIntegrity::IntegrityFeatures integrity_features =
            SubresourceIntegrityHelper::GetFeatures(&element_document);
        SubresourceIntegrity::ReportInfo report_info;
        SubresourceIntegrity::ParseIntegrityAttribute(
            integrity_attr, integrity_features, integrity_metadata, &report_info);
        SubresourceIntegrityHelper::DoReport(element_document, report_info);
    }

    // <spec step="20">Let referrer policy be the current state of the element's
    // referrerpolicy content attribute.</spec>
    String referrerpolicy_attr = element_->ReferrerPolicyAttributeValue();
    ReferrerPolicy referrer_policy = kReferrerPolicyDefault;
    if (!referrerpolicy_attr.IsEmpty()) {
        SecurityPolicy::ReferrerPolicyFromString(
            referrerpolicy_attr, kDoNotSupportReferrerPolicyLegacyKeywords,
            &referrer_policy);
    }

    // <spec step="21">Let parser metadata be "parser-inserted" if the script
    // element has been flagged as "parser-inserted", and "not-parser-inserted"
    // otherwise.</spec>
    ParserDisposition parser_state =
        IsParserInserted() ? kParserInserted : kNotParserInserted;

    if (GetScriptType() == ScriptType::kModule)
        UseCounter::Count(*context_document, WebFeature::kPrepareModuleScript);

    DCHECK(!prepared_pending_script_);

    // TODO(csharrison): This logic only works if the tokenizer/parser was not
    // blocked waiting for scripts when the element was inserted. This usually
    // fails for instance, on second document.write if a script writes twice
    // in a row. To fix this, the parser might have to keep track of raw
    // string position.
    //
    // Also PendingScript's contructor has the same code.
    const bool is_in_document_write = element_document.IsInDocumentWrite();

    // Reset line numbering for nested writes.
    TextPosition position =
        is_in_document_write ? TextPosition() : script_start_position;

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

    // <spec step="24">If the element has a src content attribute, then:</spec>
    if (element_->HasSourceAttribute()) {
        // <spec step="24.1">Let src be the value of the element's src
        // attribute.</spec>
        String src =
            StripLeadingAndTrailingHTMLSpaces(element_->SourceAttributeValue());

        // <spec step="24.2">If src is the empty string, queue a task to fire an
        // event named error at the element, and return.</spec>
        if (src.IsEmpty()) {
            element_document.GetTaskRunner(TaskType::kDOMManipulation)
                ->PostTask(FROM_HERE,
                    WTF::Bind(&ScriptElementBase::DispatchErrorEvent,
                        WrapPersistent(element_.Get())));
            return false;
        }

        // <spec step="24.3">Set the element's from an external file flag.</spec>
        is_external_script_ = true;

        // <spec step="24.4">Parse src relative to the element's node
        // document.</spec>
        KURL url = element_document.CompleteURL(src);

        // <spec step="24.5">If the previous step failed, queue a task to fire an
        // event named error at the element, and return. Otherwise, let url be the
        // resulting URL record.</spec>
        if (!url.IsValid()) {
            element_document.GetTaskRunner(TaskType::kDOMManipulation)
                ->PostTask(FROM_HERE,
                    WTF::Bind(&ScriptElementBase::DispatchErrorEvent,
                        WrapPersistent(element_.Get())));
            return false;
        }

        // <spec step="24.6">Switch on the script's type:</spec>
        if (GetScriptType() == ScriptType::kClassic) {
            // - "classic":

            // <spec step="15">If the script element has a charset attribute, then let
            // encoding be the result of getting an encoding from the value of the
            // charset attribute. If the script element does not have a charset
            // attribute, or if getting an encoding failed, let encoding be the same
            // as the encoding of the script element's node document.</spec>
            //
            // TODO(hiroshige): Should we handle failure in getting an encoding?
            WTF::TextEncoding encoding;
            if (!element_->CharsetAttributeValue().IsEmpty())
                encoding = WTF::TextEncoding(element_->CharsetAttributeValue());
            else
                encoding = element_document.Encoding();

            // <spec step="24.6.A">"classic"
            //
            // Fetch a classic script given url, settings object, options, classic
            // script CORS setting, and encoding.</spec>
            FetchClassicScript(url, element_document, options, cross_origin,
                encoding);
        }
        else {
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
    if (!element_->HasSourceAttribute()) {
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
        KURL base_url = element_document.BaseURL();

        // <spec step="25.2">Switch on the script's type:</spec>
        switch (GetScriptType()) {
            // Step 25.2.A. "classic" [spec text]
        case ScriptType::kClassic: {
            // <spec step="25.2.A.1">Let script be the result of creating a classic
            // script using source text, settings object, base URL, and
            // options.</spec>

            ScriptSourceLocationType script_location_type =
                ScriptSourceLocationType::kInline;
            if (!parser_inserted_) {
                script_location_type =
                    ScriptSourceLocationType::kInlineInsideGeneratedElement;
            }
            else if (is_in_document_write) {
                script_location_type =
                    ScriptSourceLocationType::kInlineInsideDocumentWrite;
            }

            prepared_pending_script_ = ClassicPendingScript::CreateInline(
                element_, position, script_location_type, options);

            // <spec step="25.2.A.2">Set the script's script to script.</spec>
            //
            // <spec step="25.2.A.3">The script is ready.</spec>
            //
            // Implemented by ClassicPendingScript.
            break;
        }

                                 // Step 25.2.B. "module" [spec text]
        case ScriptType::kModule: {
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
            break;
        }
        }
    }

    DCHECK(prepared_pending_script_);

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

    // <spec step="26.B">If the script's type is "classic", and the element has a
    // src attribute, and the element has been flagged as "parser-inserted", and
    // the element does not have an async attribute ...</spec>
    if (GetScriptType() == ScriptType::kClassic &&
        element_->HasSourceAttribute() && parser_inserted_ &&
        !element_->AsyncAttributeValue()) {
        // This clause is implemented by the caller-side of prepareScript():
        // - HTMLParserScriptRunner::requestParsingBlockingScript()
        // - TODO(hiroshige): Investigate XMLDocumentParser::endElementNs()
        will_be_parser_executed_ = true;

        return true;
    }

    // <spec step="26.C">If the script's type is "classic", and the element has a
    // src attribute, and the element does not have an async attribute, and the
    // element does not have the "non-blocking" flag set
    //
    // If the script's type is "module", and the element does not have an async
    // attribute, and the element does not have the "non-blocking" flag set
    // ...</spec>
    if ((GetScriptType() == ScriptType::kClassic &&
        element_->HasSourceAttribute() && !element_->AsyncAttributeValue() &&
        !non_blocking_) ||
        (GetScriptType() == ScriptType::kModule &&
            !element_->AsyncAttributeValue() && !non_blocking_)) {
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

        return true;
    }

    // <spec step="26.D">If the script's type is "classic", and the element has a
    // src attribute
    //
    // If the script's type is "module" ...</spec>
    if ((GetScriptType() == ScriptType::kClassic &&
        element_->HasSourceAttribute()) ||
        GetScriptType() == ScriptType::kModule) {
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

        return true;
    }

    // The following clauses are executed only if the script's type is "classic"
    // and the element doesn't have a src attribute.
    DCHECK_EQ(GetScriptType(), ScriptType::kClassic);
    DCHECK(!is_external_script_);

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
    if (!element_->HasSourceAttribute() && parser_inserted_ &&
        !element_document.IsScriptExecutionReady()) {
        // The former part of this clause is
        // implemented by the caller-side of prepareScript():
        // - HTMLParserScriptRunner::requestParsingBlockingScript()
        // - TODO(hiroshige): Investigate XMLDocumentParser::endElementNs()
        will_be_parser_executed_ = true;
        // <spec step="26.E">... Set the element's "ready to be parser-executed"
        // flag. ...</spec>
        ready_to_be_parser_executed_ = true;

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
    KURL script_url = (!is_in_document_write && parser_inserted_)
        ? element_document.Url()
        : KURL();
    TakePendingScript(ScriptSchedulingType::kImmediate)
        ->ExecuteScriptBlock(script_url);
#endif
    return true;
}

std::shared_ptr<PendingScript> ScriptLoader::TakePendingScript(ScriptSchedulingType schedulingType)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

}  // namespace blink
