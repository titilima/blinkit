// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_parser_script_runner.cc
// Description: HTMLParserScriptRunner Class
//      Author: Ziming Li
//     Created: 2019-10-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/script/html_parser_script_runner.h"

#include <inttypes.h>
#include <memory>
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/html/parser/html_input_stream.h"
#include "third_party/blink/renderer/core/html/parser/nesting_level_incrementer.h"
#include "third_party/blink/renderer/core/script/html_parser_script_runner_host.h"
#include "third_party/blink/renderer/core/script/ignore_destructive_write_count_incrementer.h"
#include "third_party/blink/renderer/core/script/script_element_base.h"
#include "third_party/blink/renderer/core/script/script_loader.h"
#include "third_party/blink/renderer/platform/instrumentation/tracing/trace_event.h"

namespace blink {

namespace {

void DoExecuteScript(PendingScript *pending_script, const GURL& document_url) {
  pending_script->ExecuteScriptBlock(document_url);
}

static GURL DocumentURLForScriptExecution(Document* document) {
  if (!document)
    return GURL();

  if (!document->GetFrame()) {
#ifndef BLINKIT_CRAWLER_ONLY
    if (document->ImportsController())
      return document->Url();
#endif
    return GURL();
  }

  // Use the URL of the currently active document for this frame.
  return document->GetFrame()->GetDocument()->Url();
}

}  // namespace

using namespace HTMLNames;

HTMLParserScriptRunner::HTMLParserScriptRunner(
    HTMLParserReentryPermit* reentry_permit,
    Document* document,
    HTMLParserScriptRunnerHost* host)
    : reentry_permit_(reentry_permit), document_(document), host_(host) {
  DCHECK(host_);
}

HTMLParserScriptRunner::~HTMLParserScriptRunner() {}

void HTMLParserScriptRunner::Detach() {
  if (!document_)
    return;

  if (parser_blocking_script_)
    parser_blocking_script_->Dispose();
  parser_blocking_script_ = nullptr;

  while (!scripts_to_execute_after_parsing_.IsEmpty()) {
    PendingScript* pending_script =
        scripts_to_execute_after_parsing_.TakeFirst();
    pending_script->Dispose();
  }
  document_ = nullptr;
  // m_reentryPermit is not cleared here, because the script runner
  // may continue to run pending scripts after the parser has
  // detached.
}

bool HTMLParserScriptRunner::IsParserBlockingScriptReady() {
  DCHECK(ParserBlockingScript());
  if (!document_->IsScriptExecutionReady())
    return false;
  return ParserBlockingScript()->IsReady();
}

// Corresponds to some steps of the "Otherwise" Clause of 'An end tag whose
// tag name is "script"'
// https://html.spec.whatwg.org/multipage/parsing.html#scriptEndTag
void HTMLParserScriptRunner::
    ExecutePendingParserBlockingScriptAndDispatchEvent() {
  // Stop watching loads before executeScript to prevent recursion if the script
  // reloads itself.
  // TODO(kouhei): Consider merging this w/ pendingScript->dispose() after the
  // if block.
  // TODO(kouhei, hiroshige): Consider merging this w/ the code clearing
  // |parser_blocking_script_| below.
  PendingScript* pending_script = parser_blocking_script_;
  pending_script->StopWatchingForLoad();

  if (!IsExecutingScript()) {
    // TODO(kouhei, hiroshige): Investigate why we need checkpoint here.
    // BKTODO: Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());
    // The parser cannot be unblocked as a microtask requested another
    // resource
    if (!document_->IsScriptExecutionReady())
      return;
  }

  // <spec step="B.1">Let the script be the pending
  // parsing-blocking script. There is no longer a pending parsing-blocking
  // script.</spec>
  parser_blocking_script_ = nullptr;

  {
    // <spec step="B.7">Increment the parser's script
    // nesting level by one (it should be zero before this step, so this sets it
    // to one).</spec>
    HTMLParserReentryPermit::ScriptNestingLevelIncrementer
        nesting_level_incrementer =
            reentry_permit_->IncrementScriptNestingLevel();

    // TODO(hiroshige): Remove IgnoreDestructiveWriteCountIncrementer here,
    // according to the spec. After https://crbug.com/721914 is resolved,
    // |document_| is equal to the element's context document used in
    // PendingScript::ExecuteScriptBlockInternal(), and thus this can be removed
    // more easily.
    IgnoreDestructiveWriteCountIncrementer
        ignore_destructive_write_count_incrementer(document_);

    // <spec step="B.8">Execute the script.</spec>
    DCHECK(IsExecutingScript());
    DoExecuteScript(pending_script, DocumentURLForScriptExecution(document_));

    // <spec step="B.9">Decrement the parser's script
    // nesting level by one. If the parser's script nesting level is zero (which
    // it always should be at this point), then set the parser pause flag to
    // false.</spec>
    //
    // This is implemented by ~ScriptNestingLevelIncrementer().
  }

  DCHECK(!IsExecutingScript());
}

// Should be correspond to
// https://html.spec.whatwg.org/multipage/scripting.html#execute-the-script-block
// but currently does more than specced, because historically this and
// ExecutePendingParserBlockingScriptAndDispatchEvent() was the same method.
// TODO(hiroshige): Make this spec-conformant.
void HTMLParserScriptRunner::ExecutePendingDeferredScriptAndDispatchEvent(
    PendingScript* pending_script) {
  // Stop watching loads before executeScript to prevent recursion if the script
  // reloads itself.
  // TODO(kouhei): Consider merging this w/ pendingScript->dispose() after the
  // if block.
  pending_script->StopWatchingForLoad();

  if (!IsExecutingScript()) {
    ASSERT(false); // BKTODO: Check this
#if 0
    // TODO(kouhei, hiroshige): Investigate why we need checkpoint here.
    Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());
#endif
  }

  {
    // <spec label="scriptEndTag" step="B.7">Increment the parser's script
    // nesting level by one (it should be zero before this step, so this sets it
    // to one).</spec>
    HTMLParserReentryPermit::ScriptNestingLevelIncrementer
        nesting_level_incrementer =
            reentry_permit_->IncrementScriptNestingLevel();

    IgnoreDestructiveWriteCountIncrementer
        ignore_destructive_write_count_incrementer(document_);

    // <spec label="scriptEndTag" step="B.8">Execute the script.</spec>
    DCHECK(IsExecutingScript());
    DoExecuteScript(pending_script, DocumentURLForScriptExecution(document_));

    // <spec label="scriptEndTag" step="B.9">Decrement the parser's script
    // nesting level by one. If the parser's script nesting level is zero (which
    // it always should be at this point), then set the parser pause flag to
    // false.</spec>
    //
    // This is implemented by ~ScriptNestingLevelIncrementer().
  }

  DCHECK(!IsExecutingScript());
}

void HTMLParserScriptRunner::PendingScriptFinished(
    PendingScript* pending_script) {
  // Handle cancellations of parser-blocking script loads without
  // notifying the host (i.e., parser) if these were initiated by nested
  // document.write()s. The cancellation may have been triggered by
  // script execution to signal an abrupt stop (e.g., window.close().)
  //
  // The parser is unprepared to be told, and doesn't need to be.
  if (IsExecutingScript() && pending_script->WasCanceled()) {
    pending_script->Dispose();

    DCHECK_EQ(pending_script, ParserBlockingScript());
    parser_blocking_script_ = nullptr;

    return;
  }

  host_->NotifyScriptLoaded(pending_script);
}

// 'An end tag whose tag name is "script"'
// https://html.spec.whatwg.org/multipage/parsing.html#scriptEndTag
//
// Script handling lives outside the tree builder to keep each class simple.
void HTMLParserScriptRunner::ProcessScriptElement(
    Element* script_element,
    const TextPosition& script_start_position) {
  DCHECK(script_element);

  // FIXME: If scripting is disabled, always just return.

  bool had_preload_scanner = host_->HasPreloadScanner();

  // <spec>An end tag whose tag name is "script" ...</spec>
  //
  // Try to execute the script given to us.
  ProcessScriptElementInternal(script_element, script_start_position);

  // <spec>... At this stage, if there is a pending parsing-blocking script,
  // then:</spec>
  if (HasParserBlockingScript()) {
    // <spec step="A">If the script nesting level is not zero: ...</spec>
    if (IsExecutingScript()) {
      // <spec step="A">... Set the parser pause flag to true, and abort the
      // processing of any nested invocations of the tokenizer, yielding control
      // back to the caller. (Tokenization will resume when the caller returns
      // to the "outer" tree construction stage.)</spec>
      //
      // TODO(hiroshige): set the parser pause flag to true here.

      // Unwind to the outermost HTMLParserScriptRunner::processScriptElement
      // before continuing parsing.
      return;
    }

    // - "Otherwise":

    parser_blocking_script_->MarkParserBlockingLoadStartTime();

    // If preload scanner got created, it is missing the source after the
    // current insertion point. Append it and scan.
    if (!had_preload_scanner && host_->HasPreloadScanner())
      host_->AppendCurrentInputStreamToPreloadScannerAndScan();

    ExecuteParsingBlockingScripts();
  }
}

bool HTMLParserScriptRunner::HasParserBlockingScript() const {
  return ParserBlockingScript();
}

// The "Otherwise" Clause of 'An end tag whose tag name is "script"'
// https://html.spec.whatwg.org/multipage/parsing.html#scriptEndTag
void HTMLParserScriptRunner::ExecuteParsingBlockingScripts() {
  // <spec step="B.3">If the parser's Document has a style sheet that is
  // blocking scripts or the script's "ready to be parser-executed" flag is not
  // set: spin the event loop until the parser's Document has no style sheet
  // that is blocking scripts and the script's "ready to be parser-executed"
  // flag is set.</spec>
  //
  // These conditions correspond to isParserBlockingScriptReady() and
  // if it is false, executeParsingBlockingScripts() will be called later
  // when isParserBlockingScriptReady() becomes true:
  // (1) from HTMLParserScriptRunner::executeScriptsWaitingForResources(), or
  // (2) from HTMLParserScriptRunner::executeScriptsWaitingForLoad().
  while (HasParserBlockingScript() && IsParserBlockingScriptReady()) {
    DCHECK(document_);
    DCHECK(!IsExecutingScript());
    DCHECK(document_->IsScriptExecutionReady());

    // <spec step="B.6">Let the insertion point be just before the next input
    // character.</spec>
    InsertionPointRecord insertion_point_record(host_->InputStream());

    // 1., 7.--9.
    ExecutePendingParserBlockingScriptAndDispatchEvent();

    // <spec step="B.10">Let the insertion point be undefined again.</spec>
    //
    // Implemented as ~InsertionPointRecord().

    // <spec step="B.11">If there is once again a pending parsing-blocking
    // script, then repeat these steps from step 1.</spec>
  }
}

void HTMLParserScriptRunner::ExecuteScriptsWaitingForLoad(
    PendingScript* pending_script) {
  TRACE_EVENT0("blink", "HTMLParserScriptRunner::executeScriptsWaitingForLoad");
  DCHECK(!IsExecutingScript());
  DCHECK(HasParserBlockingScript());
  DCHECK_EQ(pending_script, ParserBlockingScript());
  DCHECK(ParserBlockingScript()->IsReady());
  ExecuteParsingBlockingScripts();
}

void HTMLParserScriptRunner::ExecuteScriptsWaitingForResources() {
  TRACE_EVENT0("blink",
               "HTMLParserScriptRunner::executeScriptsWaitingForResources");
  DCHECK(document_);
  DCHECK(!IsExecutingScript());
  DCHECK(document_->IsScriptExecutionReady());
  ExecuteParsingBlockingScripts();
}

// https://html.spec.whatwg.org/multipage/parsing.html#stop-parsing
//
// <spec step="3">If the list of scripts that will execute when the document has
// finished parsing is not empty, run these substeps:</spec>
bool HTMLParserScriptRunner::ExecuteScriptsWaitingForParsing() {
  TRACE_EVENT0("blink",
               "HTMLParserScriptRunner::executeScriptsWaitingForParsing");

  while (!scripts_to_execute_after_parsing_.IsEmpty()) {
    DCHECK(!IsExecutingScript());
    DCHECK(!HasParserBlockingScript());
    DCHECK(scripts_to_execute_after_parsing_.front()->IsExternalOrModule());

    // <spec step="3.1">Spin the event loop until the first script in the list
    // of scripts that will execute when the document has finished parsing has
    // its "ready to be parser-executed" flag set and the parser's Document has
    // no style sheet that is blocking scripts.</spec>
    //
    // TODO(hiroshige): Is the latter part checked anywhere?
    if (!scripts_to_execute_after_parsing_.front()->IsReady()) {
      scripts_to_execute_after_parsing_.front()->WatchForLoad(this);
      scripts_to_execute_after_parsing_.front()
          ->MarkParserBlockingLoadStartTime();
      return false;
    }

    // <spec step="3.3">Remove the first script element from the list of scripts
    // that will execute when the document has finished parsing (i.e. shift out
    // the first entry in the list).</spec>
    PendingScript* first = scripts_to_execute_after_parsing_.TakeFirst();

    // <spec step="3.2">Execute the first script in the list of scripts that
    // will execute when the document has finished parsing.</spec>
    ExecutePendingDeferredScriptAndDispatchEvent(first);

    // FIXME: What is this m_document check for?
    if (!document_)
      return false;

    // <spec step="3.4">If the list of scripts that will execute when the
    // document has finished parsing is still not empty, repeat these substeps
    // again from substep 1.</spec>
  }
  return true;
}

// https://html.spec.whatwg.org/multipage/scripting.html#prepare-a-script
void HTMLParserScriptRunner::RequestParsingBlockingScript(
    ScriptLoader* script_loader) {
  // <spec step="25.B">... The element is the pending parsing-blocking script of
  // the Document of the parser that created the element. (There can only be one
  // such script per Document at a time.) ...</spec>
  CHECK(!ParserBlockingScript());
  parser_blocking_script_ =
      script_loader->TakePendingScript(ScriptSchedulingType::kParserBlocking);
  if (!ParserBlockingScript())
    return;

  DCHECK(ParserBlockingScript()->IsExternal());

  // We only care about a load callback if resource is not already in the cache.
  // Callers will attempt to run the m_parserBlockingScript if possible before
  // returning control to the parser.
  if (!ParserBlockingScript()->IsReady()) {
    parser_blocking_script_->StartStreamingIfPossible(std::function<void()>());
    parser_blocking_script_->WatchForLoad(this);
  }
}

// https://html.spec.whatwg.org/multipage/scripting.html#prepare-a-script
void HTMLParserScriptRunner::RequestDeferredScript(
    ScriptLoader* script_loader) {
  PendingScript* pending_script =
      script_loader->TakePendingScript(ScriptSchedulingType::kDefer);
  if (!pending_script)
    return;

  if (!pending_script->IsReady()) {
    pending_script->StartStreamingIfPossible(std::function<void()>());
  }

  DCHECK(pending_script->IsExternalOrModule());

  // <spec step="25.A">... Add the element to the end of the list of scripts
  // that will execute when the document has finished parsing associated with
  // the Document of the parser that created the element. ...</spec>
  scripts_to_execute_after_parsing_.push_back(pending_script);
}

// The initial steps for 'An end tag whose tag name is "script"'
// https://html.spec.whatwg.org/multipage/parsing.html#scriptEndTag
void HTMLParserScriptRunner::ProcessScriptElementInternal(
    Element* script,
    const TextPosition& script_start_position) {
  DCHECK(document_);
  DCHECK(!HasParserBlockingScript());
  {
    ScriptLoader* script_loader = ScriptLoaderFromElement(script);

    // FIXME: Align trace event name and function name.
    TRACE_EVENT1("blink", "HTMLParserScriptRunner::execute", "data",
                 GetTraceArgsForScriptElement(*document_, script_start_position,
                                              NullURL()));
    DCHECK(script_loader->IsParserInserted());

#if 0 // BKTODO:
    if (!IsExecutingScript())
      Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());
#endif

    // <spec>... Let the old insertion point have the same value as the current
    // insertion point. Let the insertion point be just before the next input
    // character. ...</spec>
    InsertionPointRecord insertion_point_record(host_->InputStream());

    // <spec>... Increment the parser's script nesting level by one. ...</spec>
    HTMLParserReentryPermit::ScriptNestingLevelIncrementer
        nesting_level_incrementer =
            reentry_permit_->IncrementScriptNestingLevel();

    // <spec>... Prepare the script. This might cause some script to execute,
    // which might cause new characters to be inserted into the tokenizer, and
    // might cause the tokenizer to output more tokens, resulting in a reentrant
    // invocation of the parser. ...</spec>
    script_loader->PrepareScript(script_start_position);

    // [PS]
    // https://html.spec.whatwg.org/multipage/scripting.html#prepare-a-script
    if (!script_loader->WillBeParserExecuted())
      return;

    if (script_loader->WillExecuteWhenDocumentFinishedParsing()) {
      // [PS] Step 25.A.
      RequestDeferredScript(script_loader);
    } else if (script_loader->ReadyToBeParserExecuted()) {
      // <spec label="PS" step="25.E">... it's an HTML parser whose script
      // nesting level is not greater than one, ...</spec>
      if (reentry_permit_->ScriptNestingLevel() == 1u) {
        // <spec label="PS" step="25.E">... The element is the pending
        // parsing-blocking script of the Document of the parser that created
        // the element. (There can only be one such script per Document at a
        // time.) ...</spec>
        CHECK(!parser_blocking_script_);
        parser_blocking_script_ = script_loader->TakePendingScript(
            ScriptSchedulingType::kParserBlockingInline);
      } else {
        // <spec label="PS" step="25.F">Otherwise
        //
        // Immediately execute the script block, even if other scripts are
        // already executing.</spec>
        //
        // TODO(hiroshige): Merge the block into ScriptLoader::prepareScript().
        DCHECK_GT(reentry_permit_->ScriptNestingLevel(), 1u);
        if (parser_blocking_script_)
          parser_blocking_script_->Dispose();
        parser_blocking_script_ = nullptr;
        DoExecuteScript(
            script_loader->TakePendingScript(ScriptSchedulingType::kImmediate),
            DocumentURLForScriptExecution(document_));
      }
    } else {
      // [PS] Step 25.B.
      RequestParsingBlockingScript(script_loader);
    }

    // <spec>... Decrement the parser's script nesting level by one. If the
    // parser's script nesting level is zero, then set the parser pause flag to
    // false. ...</spec>
    //
    // Implemented by ~ScriptNestingLevelIncrementer().

    // <spec>... Let the insertion point have the value of the old insertion
    // point. ...</spec>
    //
    // Implemented by ~InsertionPointRecord().
  }
}

void HTMLParserScriptRunner::Trace(blink::Visitor* visitor) {
  visitor->Trace(document_);
  visitor->Trace(host_);
  visitor->Trace(parser_blocking_script_);
  visitor->Trace(scripts_to_execute_after_parsing_);
  PendingScriptClient::Trace(visitor);
}

}  // namespace blink
