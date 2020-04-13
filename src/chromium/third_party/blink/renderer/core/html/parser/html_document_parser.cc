// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_document_parser.cc
// Description: HTMLDocumentParser Class
//      Author: Ziming Li
//     Created: 2019-10-17
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

#include "third_party/blink/renderer/core/html/parser/html_document_parser.h"

#include <memory>
#include <utility>

#include "base/auto_reset.h"
#include "base/numerics/safe_conversions.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html/parser/atomic_html_token.h"
#include "third_party/blink/renderer/core/html/parser/html_resource_preloader.h"
#include "third_party/blink/renderer/core/html/parser/html_tree_builder.h"
#include "third_party/blink/renderer/core/html/parser/nesting_level_incrementer.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/navigation_scheduler.h"
#include "third_party/blink/renderer/core/script/html_parser_script_runner.h"
// BKTODO: #include "third_party/blink/renderer/platform/cross_thread_functional.h"
// BKTODO: #include "third_party/blink/renderer/platform/heap/handle.h"
// BKTODO: #include "third_party/blink/renderer/platform/instrumentation/tracing/trace_event.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
// BKTODO: #include "third_party/blink/renderer/platform/scheduler/public/thread.h"
// BKTODO: #include "third_party/blink/renderer/platform/scheduler/public/thread_scheduler.h"
#include "third_party/blink/renderer/platform/shared_buffer.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/media_values_cached.h"
#   include "third_party/blink/renderer/core/css/resolver/style_resolver.h"
#   include "third_party/blink/renderer/core/html/html_document.h"
#   include "third_party/blink/renderer/core/loader/link_loader.h"
#endif

namespace blink {

using namespace html_names;

class PumpSession final : public NestingLevelIncrementer
{
    STACK_ALLOCATED();
public:
    PumpSession(unsigned &nestingLevel) : NestingLevelIncrementer(nestingLevel) {}
};

// This is a direct transcription of step 4 from:
// http://www.whatwg.org/specs/web-apps/current-work/multipage/the-end.html#fragment-case
static HTMLTokenizer::State TokenizerStateForContextElement(
    Element* context_element,
    bool report_errors,
    const HTMLParserOptions& options) {
  if (!context_element)
    return HTMLTokenizer::kDataState;

  const QualifiedName& context_tag = context_element->TagQName();

  if (context_tag.Matches(kTitleTag) || context_tag.Matches(kTextareaTag))
    return HTMLTokenizer::kRCDATAState;
  if (context_tag.Matches(kStyleTag) || context_tag.Matches(kXmpTag) ||
      context_tag.Matches(kIFrameTag) ||
      (context_tag.Matches(kNoscriptTag) && options.script_enabled) ||
      context_tag.Matches(kNoframesTag))
    return report_errors ? HTMLTokenizer::kRAWTEXTState
                         : HTMLTokenizer::kPLAINTEXTState;
  if (context_tag.Matches(kScriptTag))
    return report_errors ? HTMLTokenizer::kScriptDataState
                         : HTMLTokenizer::kPLAINTEXTState;
  if (context_tag.Matches(kPlaintextTag))
    return HTMLTokenizer::kPLAINTEXTState;
  return HTMLTokenizer::kDataState;
}

HTMLDocumentParser::HTMLDocumentParser(Document& document)
    : HTMLDocumentParser(document, kAllowScriptingContent) {
  script_runner_ =
      HTMLParserScriptRunner::Create(ReentryPermit(), &document, this);
  tree_builder_ =
      HTMLTreeBuilder::Create(this, document, kAllowScriptingContent, options_);
}

HTMLDocumentParser::HTMLDocumentParser(
    DocumentFragment* fragment,
    Element* context_element,
    ParserContentPolicy parser_content_policy)
    : HTMLDocumentParser(fragment->GetDocument(),
                         parser_content_policy) {
  // No script_runner_ in fragment parser.
  tree_builder_ = HTMLTreeBuilder::Create(this, fragment, context_element,
                                          parser_content_policy, options_);

  // For now document fragment parsing never reports errors.
  bool report_errors = false;
  tokenizer_->SetState(TokenizerStateForContextElement(
      context_element, report_errors, options_));
}

HTMLDocumentParser::HTMLDocumentParser(Document& document,
                                       ParserContentPolicy content_policy)
    : ScriptableDocumentParser(document, content_policy),
      options_(&document),
      reentry_permit_(HTMLParserReentryPermit::Create()),
      token_(std::make_unique<HTMLToken>()),
      tokenizer_(HTMLTokenizer::Create(options_)),
      loading_task_runner_(document.GetTaskRunner(TaskType::kNetworking)),
      preloader_(HTMLResourcePreloader::Create(document)),
      pending_csp_meta_token_(nullptr),
      end_was_delayed_(false),
      tasks_were_paused_(false),
      pump_session_nesting_level_(0),
      pump_speculations_session_nesting_level_(0),
      is_parsing_at_line_number_(false),
      tried_loading_link_headers_(false),
      added_pending_stylesheet_in_body_(false),
      is_waiting_for_stylesheets_(false) {
  DCHECK(token_ && tokenizer_);
}

HTMLDocumentParser::~HTMLDocumentParser() = default;

void HTMLDocumentParser::Dispose() {
  // Nothing to do in BlinKit.
}

void HTMLDocumentParser::Detach() {
  DocumentParser::Detach();
  if (script_runner_)
    script_runner_->Detach();
  tree_builder_->Detach();
  // FIXME: It seems wrong that we would have a preload scanner here. Yet during
  // fast/dom/HTMLScriptElement/script-load-events.html we do.
  preload_scanner_.reset();
  insertion_preload_scanner_.reset();
  // Oilpan: It is important to clear token_ to deallocate backing memory of
  // HTMLToken::data_ and let the allocator reuse the memory for
  // HTMLToken::data_ of a next HTMLDocumentParser. We need to clear
  // tokenizer_ first because tokenizer_ has a raw pointer to token_.
  tokenizer_.reset();
  token_.reset();
}

void HTMLDocumentParser::StopParsing() {
  DocumentParser::StopParsing();
}

// This kicks off "Once the user agent stops parsing" as described by:
// http://www.whatwg.org/specs/web-apps/current-work/multipage/the-end.html#the-end
void HTMLDocumentParser::PrepareToStopParsing() {
  // FIXME: It may not be correct to disable this for the background parser.
  // That means hasInsertionPoint() may not be correct in some cases.
  DCHECK(!HasInsertionPoint());

  // NOTE: This pump should only ever emit buffered character tokens.
  if (tokenizer_)
    PumpTokenizerIfPossible();

  if (IsStopped())
    return;

  DocumentParser::PrepareToStopParsing();

  // We will not have a scriptRunner when parsing a DocumentFragment.
  if (script_runner_)
    GetDocument()->SetReadyState(Document::kInteractive);

  // Setting the ready state above can fire mutation event and detach us from
  // underneath. In that case, just bail out.
  if (IsDetached())
    return;

  AttemptToRunDeferredScriptsAndEnd();
}

bool HTMLDocumentParser::IsParsingFragment() const {
  return tree_builder_->IsParsingFragment();
}

void HTMLDocumentParser::PumpTokenizerIfPossible() {
  CheckIfBodyStylesheetAdded();
  if (IsStopped() || IsPaused())
    return;

  PumpTokenizer();
}

void HTMLDocumentParser::RunScriptsForPausedTreeBuilder() {
  DCHECK(ScriptingContentIsAllowed(GetParserContentPolicy()));

  TextPosition script_start_position = TextPosition::BelowRangePosition();
  Element* script_element =
      tree_builder_->TakeScriptToProcess(script_start_position);
  // We will not have a scriptRunner when parsing a DocumentFragment.
  if (script_runner_)
    script_runner_->ProcessScriptElement(script_element, script_start_position);
  CheckIfBodyStylesheetAdded();
}

bool HTMLDocumentParser::CanTakeNextToken() {
  if (IsStopped())
    return false;

  // If we're paused waiting for a script, we try to execute scripts before
  // continuing.
  if (tree_builder_->HasParserBlockingScript())
    RunScriptsForPausedTreeBuilder();
  if (IsStopped() || IsPaused())
    return false;

  // FIXME: It's wrong for the HTMLDocumentParser to reach back to the
  // LocalFrame, but this approach is how the old parser handled stopping when
  // the page assigns window.location.  What really should happen is that
  // assigning window.location causes the parser to stop parsing cleanly.  The
  // problem is we're not perpared to do that at every point where we run
  // JavaScript.
  if (!IsParsingFragment() && GetDocument()->GetFrame() &&
      GetDocument()
          ->GetFrame()
          ->GetNavigationScheduler()
          .LocationChangePending())
    return false;

  return true;
}

void HTMLDocumentParser::EnqueueTokenizedChunk(
    std::unique_ptr<TokenizedChunk> chunk) {
  DCHECK(chunk);
  if (!IsParsing())
    return;

  // ApplicationCache needs to be initialized before issuing preloads. We
  // suspend preload until HTMLHTMLElement is inserted and ApplicationCache is
  // initialized. Note: link rel preloads don't follow this policy per the spec.
  // These directives should initiate a fetch as fast as possible.
  if (!tried_loading_link_headers_ && GetDocument()->Loader()) {
    ASSERT(false); // BKTODO:
#if 0
    // Note that on commit, the loader dispatched preloads for all the non-media
    // links.
    GetDocument()->Loader()->DispatchLinkHeaderPreloads(
        &chunk->viewport, LinkLoader::kOnlyLoadMedia);
    tried_loading_link_headers_ = true;
#endif
  }

  // Defer preloads if any of the chunks contains a <meta> csp tag.
  if (chunk->pending_csp_meta_token_index != TokenizedChunk::kNoPendingToken) {
    pending_csp_meta_token_ =
        &chunk->tokens.at(chunk->pending_csp_meta_token_index);
  }

  ASSERT(false); // BKTODO:
#if 0
  if (pending_csp_meta_token_ || !GetDocument()->documentElement()) {
    PreloadRequestStream link_rel_preloads;
    for (auto& request : chunk->preloads) {
      // Link rel preloads don't need to wait for AppCache but they
      // should probably wait for CSP.
      if (!pending_csp_meta_token_ && request->IsLinkRelPreload())
        link_rel_preloads.push_back(std::move(request));
      else
        queued_preloads_.push_back(std::move(request));
    }
    preloader_->TakeAndPreload(link_rel_preloads);
  } else {
    // We can safely assume that there are no queued preloads request after the
    // document element is available, as we empty the queue immediately after
    // the document element is created in documentElementAvailable().
    DCHECK(queued_preloads_.IsEmpty());
    preloader_->TakeAndPreload(chunk->preloads);
  }

  speculations_.push_back(std::move(chunk));

  if (!IsPaused() && !IsScheduledForUnpause()) {
    if (tasks_were_paused_)
      parser_scheduler_->ForceUnpauseAfterYield();
    else
      parser_scheduler_->ScheduleForUnpause();
  }
#endif
}

void HTMLDocumentParser::DidReceiveEncodingDataFromBackgroundParser(
    const DocumentEncodingData& data) {
  GetDocument()->SetEncodingData(data);
}

void HTMLDocumentParser::ForcePlaintextForTextDocument() {
  tokenizer_->SetState(HTMLTokenizer::kPLAINTEXTState);
}

void HTMLDocumentParser::PumpTokenizer() {
  DCHECK(!IsStopped());
  DCHECK(tokenizer_);
  DCHECK(token_);

  PumpSession session(pump_session_nesting_level_);

  while (CanTakeNextToken()) {
    if (!tokenizer_->NextToken(input_.Current(), Token()))
      break;

    ConstructTreeFromHTMLToken();
    DCHECK(IsStopped() || Token().IsUninitialized());
  }

  if (IsStopped())
    return;

  // There should only be PendingText left since the tree-builder always flushes
  // the task queue before returning. In case that ever changes, crash.
  tree_builder_->Flush(kFlushAlways);
  CHECK(!IsStopped());

  if (IsPaused()) {
    DCHECK_EQ(tokenizer_->GetState(), HTMLTokenizer::kDataState);

    DCHECK(preloader_);
    // TODO(kouhei): preloader_ should be always available for synchronous
    // parsing case, adding paranoia if for speculative crash fix for
    // crbug.com/465478
    if (preloader_) {
      if (!preload_scanner_) {
        preload_scanner_ = CreatePreloadScanner(
            TokenPreloadScanner::ScannerType::kMainDocument);
        preload_scanner_->AppendToEnd(input_.Current());
      }
      ScanAndPreload(preload_scanner_.get());
    }
  }
}

void HTMLDocumentParser::ConstructTreeFromHTMLToken() {
  AtomicHTMLToken atomic_token(Token());

  // We clear the token_ in case ConstructTreeFromAtomicToken
  // synchronously re-enters the parser. We don't clear the token immedately
  // for kCharacter tokens because the AtomicHTMLToken avoids copying the
  // characters by keeping a pointer to the underlying buffer in the
  // HTMLToken. Fortunately, kCharacter tokens can't cause us to re-enter
  // the parser.
  //
  // FIXME: Stop clearing the token_ once we start running the parser off
  // the main thread or once we stop allowing synchronous JavaScript
  // execution from ParseAttribute.
  if (Token().GetType() != HTMLToken::kCharacter)
    Token().Clear();

  tree_builder_->ConstructTree(&atomic_token);
  CheckIfBodyStylesheetAdded();

  // FIXME: ConstructTree may synchronously cause Document to be detached.
  if (!token_)
    return;

  if (!Token().IsUninitialized()) {
    DCHECK_EQ(Token().GetType(), HTMLToken::kCharacter);
    Token().Clear();
  }
}

void HTMLDocumentParser::ConstructTreeFromCompactHTMLToken(
    const CompactHTMLToken& compact_token) {
  AtomicHTMLToken token(compact_token);
  tree_builder_->ConstructTree(&token);
  CheckIfBodyStylesheetAdded();
}

bool HTMLDocumentParser::HasInsertionPoint() {
  // FIXME: The wasCreatedByScript() branch here might not be fully correct. Our
  // model of the EOF character differs slightly from the one in the spec
  // because our treatment is uniform between network-sourced and script-sourced
  // input streams whereas the spec treats them differently.
  return input_.HasInsertionPoint() ||
         (WasCreatedByScript() && !input_.HaveSeenEndOfFile());
}

void HTMLDocumentParser::insert(const String& source) {
  if (IsStopped())
    return;

  if (!tokenizer_) {
    DCHECK(!InPumpSession());
    DCHECK(WasCreatedByScript());
    token_ = std::make_unique<HTMLToken>();
    tokenizer_ = HTMLTokenizer::Create(options_);
  }

  SegmentedString excluded_line_number_source(source);
  excluded_line_number_source.SetExcludeLineNumbers();
  input_.InsertAtCurrentInsertionPoint(excluded_line_number_source);
  PumpTokenizerIfPossible();

  if (IsPaused()) {
    // Check the document.write() output with a separate preload scanner as
    // the main scanner can't deal with insertions.
    if (!insertion_preload_scanner_) {
      insertion_preload_scanner_ =
          CreatePreloadScanner(TokenPreloadScanner::ScannerType::kInsertion);
    }
    insertion_preload_scanner_->AppendToEnd(source);
    ScanAndPreload(insertion_preload_scanner_.get());
  }

  EndIfDelayed();
}

void HTMLDocumentParser::Append(const String& input_source) {
  if (IsStopped())
    return;

  const SegmentedString source(input_source);

#if 0 // BKTODO: Check if necessary
  if (GetDocument()->IsPrefetchOnly()) {
    // Do not prefetch if there is an appcache.
    if (GetDocument()->Loader()->GetResponse().AppCacheID() != 0)
      return;

    if (!preload_scanner_) {
      preload_scanner_ =
          CreatePreloadScanner(TokenPreloadScanner::ScannerType::kMainDocument);
    }

    preload_scanner_->AppendToEnd(source);
    ScanAndPreload(preload_scanner_.get());

    // Return after the preload scanner, do not actually parse the document.
    return;
  }

  if (preload_scanner_) {
    if (input_.Current().IsEmpty() && !IsPaused()) {
      // We have parsed until the end of the current input and so are now moving
      // ahead of the preload scanner. Clear the scanner so we know to scan
      // starting from the current input point if we block again.
      preload_scanner_.reset();
    } else {
      preload_scanner_->AppendToEnd(source);
      if (IsPaused())
        ScanAndPreload(preload_scanner_.get());
    }
  }
#endif

  input_.AppendToEnd(source);

  if (InPumpSession()) {
    // We've gotten data off the network in a nested write. We don't want to
    // consume any more of the input stream now.  Do not worry.  We'll consume
    // this data in a less-nested write().
    return;
  }

  PumpTokenizerIfPossible();

  EndIfDelayed();
}

void HTMLDocumentParser::end() {
  DCHECK(!IsDetached());

  std::shared_ptr<HTMLDocumentParser> protect(shared_from_this());

  // Informs the the rest of WebCore that parsing is really finished (and
  // deletes this).
  tree_builder_->Finished();

#if 0 // BKTODO: Check if necessary.
  // All preloads should be done.
  preloader_.Clear();
#endif

  DocumentParser::StopParsing();
}

void HTMLDocumentParser::AttemptToRunDeferredScriptsAndEnd() {
  DCHECK(IsStopping());
  // FIXME: It may not be correct to disable this for the background parser.
  // That means hasInsertionPoint() may not be correct in some cases.
  DCHECK(!HasInsertionPoint());
  if (script_runner_ && !script_runner_->ExecuteScriptsWaitingForParsing())
    return;
  end();
}

void HTMLDocumentParser::AttemptToEnd() {
  // finish() indicates we will not receive any more data. If we are waiting on
  // an external script to load, we can't finish parsing quite yet.

  if (ShouldDelayEnd()) {
    end_was_delayed_ = true;
    return;
  }
  PrepareToStopParsing();
}

void HTMLDocumentParser::EndIfDelayed() {
  // If we've already been detached, don't bother ending.
  if (IsDetached())
    return;

  if (!end_was_delayed_ || ShouldDelayEnd())
    return;

  end_was_delayed_ = false;
  PrepareToStopParsing();
}

void HTMLDocumentParser::Finish() {
  // FIXME: We should DCHECK(!parser_stopped_) here, since it does not makes
  // sense to call any methods on DocumentParser once it's been stopped.
  // However, FrameLoader::Stop calls DocumentParser::Finish unconditionally.

  Flush();
  if (IsDetached())
    return;

  if (!tokenizer_) {
    DCHECK(!token_);
    // We're finishing before receiving any data. Rather than booting up the
    // background parser just to spin it down, we finish parsing synchronously.
    token_ = std::make_unique<HTMLToken>();
    tokenizer_ = HTMLTokenizer::Create(options_);
  }

  // We're not going to get any more data off the network, so we tell the input
  // stream we've reached the end of file. finish() can be called more than
  // once, if the first time does not call end().
  if (!input_.HaveSeenEndOfFile())
    input_.MarkEndOfFile();

  AttemptToEnd();
}

bool HTMLDocumentParser::IsExecutingScript() const {
  if (!script_runner_)
    return false;
  return script_runner_->IsExecutingScript();
}

bool HTMLDocumentParser::IsParsingAtLineNumber() const {
  return is_parsing_at_line_number_ &&
         ScriptableDocumentParser::IsParsingAtLineNumber();
}

OrdinalNumber HTMLDocumentParser::LineNumber() const {
  return input_.Current().CurrentLine();
}

TextPosition HTMLDocumentParser::GetTextPosition() const {
  const SegmentedString& current_string = input_.Current();
  OrdinalNumber line = current_string.CurrentLine();
  OrdinalNumber column = current_string.CurrentColumn();

  return TextPosition(line, column);
}

bool HTMLDocumentParser::IsWaitingForScripts() const {
  // When the TreeBuilder encounters a </script> tag, it returns to the
  // HTMLDocumentParser where the script is transfered from the treebuilder to
  // the script runner. The script runner will hold the script until its loaded
  // and run. During any of this time, we want to count ourselves as "waiting
  // for a script" and thus run the preload scanner, as well as delay completion
  // of parsing.
  bool tree_builder_has_blocking_script =
      tree_builder_->HasParserBlockingScript();
  bool script_runner_has_blocking_script =
      script_runner_ && script_runner_->HasParserBlockingScript();
  // Since the parser is paused while a script runner has a blocking script, it
  // should never be possible to end up with both objects holding a blocking
  // script.
  DCHECK(
      !(tree_builder_has_blocking_script && script_runner_has_blocking_script));
  // If either object has a blocking script, the parser should be paused.
  return tree_builder_has_blocking_script ||
         script_runner_has_blocking_script ||
         reentry_permit_->ParserPauseFlag();
}

void HTMLDocumentParser::ResumeParsingAfterPause() {
  DCHECK(!IsExecutingScript());
  DCHECK(!IsPaused());

  CheckIfBodyStylesheetAdded();
  if (IsPaused())
    return;

  insertion_preload_scanner_.reset();
  if (tokenizer_) {
    PumpTokenizerIfPossible();
  }
  EndIfDelayed();
}

void HTMLDocumentParser::AppendCurrentInputStreamToPreloadScannerAndScan() {
  DCHECK(preload_scanner_);
  preload_scanner_->AppendToEnd(input_.Current());
  ScanAndPreload(preload_scanner_.get());
}

void HTMLDocumentParser::NotifyScriptLoaded(PendingScript* pending_script) {
  DCHECK(script_runner_);
  DCHECK(!IsExecutingScript());

  if (IsStopped()) {
    return;
  }

  if (IsStopping()) {
    AttemptToRunDeferredScriptsAndEnd();
    return;
  }

  script_runner_->ExecuteScriptsWaitingForLoad(pending_script);
  if (!IsPaused())
    ResumeParsingAfterPause();
}

void HTMLDocumentParser::ExecuteScriptsWaitingForResources() {
  if (IsStopped())
    return;

  DCHECK(GetDocument()->IsScriptExecutionReady());

  if (is_waiting_for_stylesheets_)
    is_waiting_for_stylesheets_ = false;

  // Document only calls this when the Document owns the DocumentParser so this
  // will not be called in the DocumentFragment case.
  DCHECK(script_runner_);
  script_runner_->ExecuteScriptsWaitingForResources();
  if (!IsPaused())
    ResumeParsingAfterPause();
}

void HTMLDocumentParser::DidAddPendingStylesheetInBody() {
  // When in-body CSS doesn't block painting, the parser needs to pause so that
  // the DOM doesn't include any elements that may depend on the CSS for style.
  // The stylesheet can be added and removed during the parsing of a single
  // token so don't actually set the bit to block parsing here, just track
  // the state of the added sheet in case it does persist beyond a single
  // token.
  if (RuntimeEnabledFeatures::CSSInBodyDoesNotBlockPaintEnabled())
    added_pending_stylesheet_in_body_ = true;
}

void HTMLDocumentParser::DidLoadAllBodyStylesheets() {
  // Just toggle the stylesheet flag here (mostly for synchronous sheets).
  // The document will also call into executeScriptsWaitingForResources
  // which is when the parser will re-start, otherwise it will attempt to
  // resume twice which could cause state machine issues.
  added_pending_stylesheet_in_body_ = false;
}

void HTMLDocumentParser::CheckIfBodyStylesheetAdded() {
  if (added_pending_stylesheet_in_body_) {
    added_pending_stylesheet_in_body_ = false;
    is_waiting_for_stylesheets_ = true;
  }
}

void HTMLDocumentParser::ParseDocumentFragment(
    const String& source,
    DocumentFragment* fragment,
    Element* context_element,
    ParserContentPolicy parser_content_policy) {
  std::shared_ptr<HTMLDocumentParser> parser = HTMLDocumentParser::Create(
      fragment, context_element, parser_content_policy);
  parser->Append(source);
  parser->Finish();
  // Allows ~DocumentParser to assert it was detached before destruction.
  parser->Detach();
}

void HTMLDocumentParser::PauseScheduledTasks() {
  DCHECK(!tasks_were_paused_);
  tasks_were_paused_ = true;
}

void HTMLDocumentParser::UnpauseScheduledTasks() {
  DCHECK(tasks_were_paused_);
  tasks_were_paused_ = false;
}

void HTMLDocumentParser::AppendBytes(const char* data, size_t length) {
  if (!length || IsStopped())
    return;

  DecodedDataDocumentParser::AppendBytes(data, length);
}

void HTMLDocumentParser::Flush() {
  // If we've got no decoder, we never received any data.
  if (IsDetached() || NeedsDecoder())
    return;

  DecodedDataDocumentParser::Flush();
}

void HTMLDocumentParser::SetDecoder(
    std::unique_ptr<TextResourceDecoder> decoder) {
  DCHECK(decoder);
  DecodedDataDocumentParser::SetDecoder(std::move(decoder));
}

void HTMLDocumentParser::DocumentElementAvailable() {
  DCHECK(GetDocument()->documentElement());
  FetchQueuedPreloads();
}

std::unique_ptr<HTMLPreloadScanner> HTMLDocumentParser::CreatePreloadScanner(
    TokenPreloadScanner::ScannerType scanner_type) {
  return HTMLPreloadScanner::Create(
      options_, GetDocument()->Url(),
      CachedDocumentParameters::Create(GetDocument()),
      MediaValuesCached::MediaValuesCachedData(*GetDocument()), scanner_type);
}

void HTMLDocumentParser::ScanAndPreload(HTMLPreloadScanner* scanner) {
  PreloadRequestStream requests =
      scanner->Scan(GetDocument()->ValidBaseElementURL(), nullptr);
  preloader_->TakeAndPreload(requests);
}

void HTMLDocumentParser::FetchQueuedPreloads() {
  if (pending_csp_meta_token_ || !GetDocument()->documentElement())
    return;

  if (!queued_preloads_.empty())
    preloader_->TakeAndPreload(queued_preloads_);
}

}  // namespace blink
