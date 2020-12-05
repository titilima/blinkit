// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document.h
// Description: Document Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_DOCUMENT_H
#define BLINKIT_BLINK_DOCUMENT_H

#pragma once

#include <stack>
#include <unordered_set>
#include <vector>
#include "third_party/blink/renderer/core/dom/container_node.h"
#include "third_party/blink/renderer/core/dom/create_element_flags.h"
#include "third_party/blink/renderer/core/dom/document_encoding_data.h"
#include "third_party/blink/renderer/core/dom/document_lifecycle.h"
#include "third_party/blink/renderer/core/dom/document_shutdown_notifier.h"
#include "third_party/blink/renderer/core/dom/document_shutdown_observer.h"
#include "third_party/blink/renderer/core/dom/live_node_list_registry.h"
#include "third_party/blink/renderer/core/dom/mutation_observer_options.h"
#include "third_party/blink/renderer/core/dom/synchronous_mutation_notifier.h"
#include "third_party/blink/renderer/core/dom/synchronous_mutation_observer.h"
#include "third_party/blink/renderer/core/dom/tree_scope.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/timer.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/dom/text_link_colors.h"
#   include "third_party/blink/renderer/core/dom/user_action_element_set.h"
#endif

namespace blink {

class Comment;
class DocumentFragment;
class DocumentInit;
class DocumentLoader;
class DocumentParser;
class DocumentType;
class ElementDataCache;
class LocalDOMWindow;
class LocalFrame;
class Location;
class NthIndexCache;
class ResourceFetcher;
class ScriptableDocumentParser;
class ScriptElementBase;
class ScriptRunner;
class SelectorQueryCache;
class Text;
#ifndef BLINKIT_CRAWLER_ONLY
class AnimationClock;
class CSSStyleSheet;
class DocumentTimeline;
class HTMLBodyElement;
class HTMLImportsController;
class LayoutView;
class LocalFrameView;
class MediaQueryMatcher;
class Page;
enum class PageVisibilityState;
class PropertyRegistry;
class ReattachLegacyLayoutObjectList;
class RootScrollerController;
class SnapCoordinator;
class StyleEngine;
class StyleResolver;
class TextAutosizer;
class V0CustomElementRegistrationContext;
class ViewportData;
class VisitedLinkState;
#endif

enum NodeListInvalidationType : int {
    kDoNotInvalidateOnAttributeChanges = 0,
    kInvalidateOnClassAttrChange,
    kInvalidateOnIdNameAttrChange,
    kInvalidateOnNameAttrChange,
    kInvalidateOnForAttrChange,
    kInvalidateForFormControls,
    kInvalidateOnHRefAttrChange,
    kInvalidateOnAnyAttrChange,
};

const int kNumNodeListInvalidationTypes = kInvalidateOnAnyAttrChange + 1;

enum ShadowCascadeOrder {
    kShadowCascadeNone,
    kShadowCascadeV0,
    kShadowCascadeV1
};

class Document : public ContainerNode
               , public ExecutionContext
               , public TreeScope
               , public DocumentShutdownNotifier
               , public SynchronousMutationNotifier
#ifndef BLINKIT_CRAWLER_ONLY
               , public Supplementable<Document>
#endif
{
public:
    ~Document(void) override;

    void Initialize(void);
    virtual void Shutdown(void);
    bool IsHTMLDocument(void) const { return true; } // Just a placeholder.
    void Trace(Visitor *visitor) override;

    uint64_t DomTreeVersion(void) const { return m_domTreeVersion; }
    void IncDOMTreeVersion(void)
    {
        ASSERT(m_lifecycle.StateAllowsTreeMutations());
        m_domTreeVersion = ++m_globalTreeVersion;
    }

    LocalDOMWindow* domWindow(void) const { return m_domWindow; }
    void ClearDOMWindow(void) { m_domWindow = nullptr; }
    LocalFrame* GetFrame(void) const { return m_frame; }  // can be null
    LocalFrame* ExecutingFrame(void);
    LocalDOMWindow* ExecutingWindow(void) const final;
    Document* ContextDocument(void) const;
    ScriptRunner* GetScriptRunner(void) { return m_scriptRunner.get(); }
    DocumentLifecycle& Lifecycle(void) { return m_lifecycle; }
#ifndef BLINKIT_CRAWLER_ONLY
    LayoutView* GetLayoutView(void) const { return m_layoutView; }
    const PropertyRegistry* GetPropertyRegistry(void) const;
    PropertyRegistry* GetPropertyRegistry(void);
    Page* GetPage(void) const;                           // can be null
    LocalFrameView* View(void) const;                    // can be null
    TextAutosizer* GetTextAutosizer(void);
    StyleResolver* GetStyleResolver(void) const;
    StyleResolver& EnsureStyleResolver(void) const;
    AnimationClock& GetAnimationClock(void);
    DocumentTimeline& Timeline(void) const { return *m_timeline; }
    VisitedLinkState& GetVisitedLinkState(void) const { return *m_visitedLinkState; }
    UserActionElementSet& UserActionElements(void) { return m_userActionElements; }
    const UserActionElementSet& UserActionElements(void) const { return m_userActionElements; }
    // to get visually ordered hebrew and arabic pages right
    bool VisuallyOrdered(void) const { return m_visuallyOrdered; }
    float DevicePixelRatio(void) const;
    void SetupFontBuilder(ComputedStyle &documentStyle);
    // Decide which element is to define the viewport's overflow policy. If
    // |rootStyle| is set, use that as the style for the root element, rather than
    // obtaining it on our own. The reason for this is that style may not have
    // been associated with the elements yet - in which case it may have been
    // calculated on the fly (without associating it with the actual element)
    // somewhere.
    Element* ViewportDefiningElement(const ComputedStyle *rootStyle = nullptr) const;
    V0CustomElementRegistrationContext* RegistrationContext(void) const
    {
        return m_registrationContext.get();
    }
    SnapCoordinator* GetSnapCoordinator(void);
#endif

    // Exports for JS
    Element* body(void) const;
    String cookie(ExceptionState &exceptionState) const;
    DocumentFragment* createDocumentFragment(void);
    Comment* createComment(const String &data);
    Element* createElement(const AtomicString &name, ExceptionState &exceptionState);
    Text* createTextNode(const String &data);
    Element* documentElement(void) const { return m_documentElement.Get(); }
    using TreeScope::getElementById;
    Element* head(void) const;
    Location* location(void) const;
    void open(Document *enteredDocument, ExceptionState &exceptionState);
    String readyState(void) const;
    void setCookie(const String &value, ExceptionState &exceptionState);
    // Bind the url to document.url, if unavailable bind to about:blank.
    GURL urlForBinding(void) const;
    void write(const String &text, Document *enteredDocument = nullptr, ExceptionState &exceptionState = ASSERT_NO_EXCEPTION);
    void write(LocalDOMWindow *callingWindow, const std::vector<std::string> &text, ExceptionState &exceptionState);
    void writeln(const String &text, Document *enteredDocument = nullptr, ExceptionState &exceptionState = ASSERT_NO_EXCEPTION);
    void writeln(LocalDOMWindow *callingWindow, const std::vector<std::string> &text, ExceptionState &exceptionState);

    void SetDoctype(DocumentType *docType);

    // Return the document URL, or an empty URL if it's unavailable.
    // This is not an implementation of web-exposed Document.prototype.URL.
    const GURL& Url(void) const final { return m_URL; }
    void SetURL(const GURL &url);
    GURL ValidBaseElementURL(void) const;
    // Creates URL based on passed relative url and passed base URL override.
    GURL CompleteURLWithOverride(const String &url, const GURL &baseUrlOverride) const;
    // Fallback base URL.
    // https://html.spec.whatwg.org/multipage/urls-and-fetching.html#fallback-base-url
    GURL FallbackBaseURL(void) const;
    const GURL& CookieURL(void) const { return m_cookieURL; }

    const AtomicString& ContentLanguage(void) const { return m_contentLanguage; }
    void SetContentLanguage(const AtomicString &language);

    // The following implements the rule from HTML 4 for what valid names are.
    // To get this right for all the XML cases, we probably have to improve this
    // or move it and make it sensitive to the type of document.
    static bool IsValidName(const String &name);
    virtual Element* CreateElement(const AtomicString &name, CreateElementFlags flags) = 0;

    void SetTitleElement(Element *titleElement);
    void RemoveTitle(Element *titleElement);

    // keep track of what types of event listeners are registered, so we don't
    // dispatch events unnecessarily
    enum ListenerType {
        kDOMSubtreeModifiedListener = 1,
        kDOMNodeInsertedListener = 1 << 1,
        kDOMNodeRemovedListener = 1 << 2,
        kDOMNodeRemovedFromDocumentListener = 1 << 3,
        kDOMNodeInsertedIntoDocumentListener = 1 << 4,
        kDOMCharacterDataModifiedListener = 1 << 5,
        kAnimationEndListener = 1 << 6,
        kAnimationStartListener = 1 << 7,
        kAnimationIterationListener = 1 << 8,
        kTransitionEndListener = 1 << 9,
        kScrollListener = 1 << 10,
        kLoadListenerAtCapturePhaseOrAtStyleElement = 1 << 11
        // 4 bits remaining
    };
    bool HasListenerType(ListenerType listenerType) const { return 0 != (m_listenerTypes & listenerType); }
    void AddListenerType(ListenerType listenerType) { m_listenerTypes |= listenerType; }
    void AddListenerTypeIfNeeded(const AtomicString &eventType, EventTarget &eventTarget);
    void AddMutationEventListenerTypeIfEnabled(ListenerType listenerType);

    bool HasMutationObservers(void) const { return 0 != m_mutationObserverTypes; }
    bool HasMutationObserversOfType(MutationType type) const { return 0 != (m_mutationObserverTypes & type); }

    int NodeCount(void) const { return m_nodeCount; }
    void IncrementNodeCount(void) { ++m_nodeCount; }
    void DecrementNodeCount(void)
    {
        ASSERT(m_nodeCount > 0);
        --m_nodeCount;
    }

    ElementDataCache* GetElementDataCache(void) { return m_elementDataCache.Get(); }
    SelectorQueryCache& GetSelectorQueryCache(void);

    NthIndexCache* GetNthIndexCache(void) const { return m_nthIndexCache; }
    void SetNthIndexCache(NthIndexCache *nthIndexCache)
    {
        ASSERT(nullptr == m_nthIndexCache || nullptr == nthIndexCache);
        m_nthIndexCache = nthIndexCache;
    }

    void RegisterNodeList(const LiveNodeListBase *list);

    bool MayContainV0Shadow(void) const { return m_mayContainV0Shadow; }

    bool ShouldInvalidateNodeListCaches(const QualifiedName *attrName = nullptr) const;
    void InvalidateNodeListCaches(const QualifiedName *attrName);

    DocumentLoader* Loader(void) const;

    enum CompatibilityMode { kQuirksMode, kLimitedQuirksMode, kNoQuirksMode };
    CompatibilityMode GetCompatibilityMode(void) const { return m_compatibilityMode; }
    bool InQuirksMode(void) const { return kQuirksMode == m_compatibilityMode; }
    bool InNoQuirksMode(void) const { return kNoQuirksMode == m_compatibilityMode; }
    void SetCompatibilityMode(CompatibilityMode mode);

    const WTF::TextEncoding& Encoding(void) const { return m_encodingData.Encoding(); }
    void SetEncodingData(const DocumentEncodingData &newData);

    enum DocumentReadyState { kLoading, kInteractive, kComplete };
    void SetReadyState(DocumentReadyState readyState);

    enum ParsingState { kParsing, kInDOMContentLoaded, kFinishedParsing };
    bool Parsing(void) const { return kParsing == m_parsingState; }
    bool HasFinishedParsing(void) const { return kFinishedParsing == m_parsingState; }
    void SetParsingState(ParsingState parsingState);
    void ParserInsertedHtmlElement(Element &element);
    void WillInsertBody(void);
    void FinishedParsing(void);
    void CancelParsing(void);
    void Abort(void);
    void CheckCompleted(void);

    void DispatchUnloadEvents(void);

    bool IsActive(void) const { return m_lifecycle.IsActive(); }
    bool InStyleRecalc(void) const { return m_lifecycle.GetState() == DocumentLifecycle::kInStyleRecalc; }

    enum PageDismissalType {
        kNoDismissal,
        kBeforeUnloadDismissal,
        kPageHideDismissal,
        kUnloadVisibilityChangeDismissal,
        kUnloadDismissal
    };
    PageDismissalType PageDismissalEventBeingDispatched(void) const;

    bool WasDiscarded(void) const { return m_wasDiscarded; }
    void SetWasDiscarded(bool wasDiscarded) { m_wasDiscarded = wasDiscarded; }

    enum LoadEventProgress {
        kLoadEventNotRun,
        kLoadEventInProgress,
        kLoadEventCompleted,
        kBeforeUnloadEventInProgress,
        kBeforeUnloadEventCompleted,
        kPageHideInProgress,
        kUnloadVisibilityChangeInProgress,
        kUnloadEventInProgress,
        kUnloadEventHandled
    };
    bool LoadEventFinished(void) const { return m_loadEventProgress >= kLoadEventCompleted; }
    bool LoadEventStillNeeded(void) const { return kLoadEventNotRun == m_loadEventProgress; }
    void SuppressLoadEvent(void);
    // Used to allow element that loads data without going through a FrameLoader
    // to delay the 'load' event.
    void IncrementLoadEventDelayCount(void) { ++m_loadEventDelayCount; }
    void DecrementLoadEventDelayCount(void);
    void CheckLoadEventSoon(void);
    bool IsDelayingLoadEvent(void) { return 0 != m_loadEventDelayCount; }

    DocumentParser* Parser(void) const { return m_parser.Get(); }
    ScriptableDocumentParser* GetScriptableDocumentParser(void) const;
    DocumentParser* OpenForNavigation(const AtomicString &mimeType, const AtomicString &encoding);
    DocumentParser* ImplicitOpen(void);
    DocumentParser* CreateParser(void);

    enum HttpRefreshType { kHttpRefreshFromHeader, kHttpRefreshFromMetaTag };
    void MaybeHandleHttpRefresh(const String &content, HttpRefreshType refreshType);

    bool IsScriptExecutionReady(void) const {
        return HaveImportsLoaded() && HaveScriptBlockingStylesheetsLoaded();
    }

    bool CanAcceptChild(const Node &newChild, const Node *next, const Node *oldChild, ExceptionState &exceptionState) const;
    // nodeChildrenWillBeRemoved is used when removing all node children at once.
    void NodeChildrenWillBeRemoved(ContainerNode &container);
    // nodeWillBeRemoved is only safe when removing one node at a time.
    void NodeWillBeRemoved(Node &n);

    ShadowCascadeOrder GetShadowCascadeOrder(void) const { return m_shadowCascadeOrder; }
    bool ContainsV1ShadowTree(void) const { return ShadowCascadeOrder::kShadowCascadeV1 == m_shadowCascadeOrder; }

    bool IsInDocumentWrite(void) const { return m_writeRecursionDepth > 0; }

    void PushCurrentScript(ScriptElementBase *newCurrentScript);
    void PopCurrentScript(ScriptElementBase *script);

    // ExecutionContext overrides
    const GURL& BaseURL(void) const override;
    // Creates URL based on passed relative url and this documents base URL.
    // Depending on base URL value it is possible that parent document
    // base URL will be used instead. Uses CompleteURLWithOverride internally.
    GURL CompleteURL(const String &url) const final;
    ResourceFetcher* Fetcher(void) const override { return m_fetcher.Get(); }
    bool CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) override;
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType type) override;

    // Temporary flag for some UseCounter items. crbug.com/859391.
    enum class InDOMNodeRemovedHandlerState {
        kNone,
        kDOMNodeRemoved,
        kDOMNodeRemovedFromDocument
    };
    void SetInDOMNodeRemovedHandlerState(InDOMNodeRemovedHandlerState state) { m_inDomNodeRemovedHandlerState = state; }
    InDOMNodeRemovedHandlerState GetInDOMNodeRemovedHandlerState(void) const { return m_inDomNodeRemovedHandlerState; }
    bool InDOMNodeRemovedHandler(void) const { return m_inDomNodeRemovedHandlerState != InDOMNodeRemovedHandlerState::kNone; }
    void CountDetachingNodeAccessInDOMNodeRemovedHandler(void) { ASSERT(GetInDOMNodeRemovedHandlerState() != InDOMNodeRemovedHandlerState::kNone); } // Just a placeholder

#ifndef BLINKIT_CRAWLER_ONLY
    PageVisibilityState GetPageVisibilityState(void) const;

    Element* FocusedElement(void) const { return m_focusedElement.Get(); }
    Element* HoverElement(void) const { return m_hoverElement.Get(); }
    // "HTML body element" as defined by CSSOM View spec
    // (https://drafts.csswg.org/cssom-view/#the-html-body-element).
    // That is, the first body child of the document element.
    HTMLBodyElement* FirstBodyElement(void) const;

    void RemoveFocusedElementOfSubtree(Node *node, bool amongChildrenOnly = false);

    scoped_refptr<ComputedStyle> StyleForElementIgnoringPendingStylesheets(Element *element);

    bool HasNodesWithPlaceholderStyle(void) const { return m_hasNodesWithPlaceholderStyle; }
    void SetHasNodesWithPlaceholderStyle(void) { m_hasNodesWithPlaceholderStyle = true; }

    bool HasViewportUnits(void) const { return m_hasViewportUnits; }
    void SetHasViewportUnits(void) { m_hasViewportUnits = true; }
    void SetResizedForViewportUnits(void);
    void ClearResizedForViewportUnits(void);

    // A non-null template_document_host_ implies that |this| was created by
    // EnsureTemplateDocument().
    bool IsTemplateDocument(void) const { return !!m_templateDocumentHost; }

    bool IsRenderingReady(void) const;

    enum PendingSheetLayout {
        kNoLayoutWithPendingSheets,
        kDidLayoutWithPendingSheets,
        kIgnoreLayoutWithPendingSheets
    };
    bool DidLayoutWithPendingStylesheets(void) const { return m_pendingSheetLayout == kDidLayoutWithPendingSheets; }
    bool IgnoreLayoutWithPendingStylesheets(void) const { return m_pendingSheetLayout == kIgnoreLayoutWithPendingSheets; }

    bool ShouldScheduleLayout(void) const;
    bool NeedsLayoutTreeUpdate(void) const;
    void UpdateStyleAndLayoutTree(void);
    void UpdateStyleAndLayoutTreeForNode(const Node *node);
    void ScheduleLayoutTreeUpdateIfNeeded(void);
    void LayoutUpdated(void);

    RootScrollerController& GetRootScrollerController(void) const
    {
        ASSERT(m_rootScrollerController);
        return *m_rootScrollerController;
    }
    StyleEngine& GetStyleEngine(void)
    {
        ASSERT(m_styleEngine);
        return *m_styleEngine;
    }
    bool HasPendingForcedStyleRecalc(void) const;
    void UpdateActiveStyle(void);

    void EvaluateMediaQueryList(void);

    TextLinkColors& GetTextLinkColors(void) { return m_textLinkColors; }
    const TextLinkColors& GetTextLinkColors(void) const { return m_textLinkColors; }

    // TODO(layout-dev): Once everything are LayoutNG, we can get rid of this.
    ReattachLegacyLayoutObjectList& GetReattachLegacyLayoutObjectList(void);

    ViewportData& GetViewportData(void) const { return *m_viewportData; }

    void EnqueueVisualViewportResizeEvent(void);

#   if DCHECK_IS_ON()
    unsigned& SlotAssignmentRecalcForbiddenRecursionDepth(void) { return m_slotAssignmentRecalcForbiddenRecursionDepth; }
    bool IsSlotAssignmentRecalcForbidden(void) { return m_slotAssignmentRecalcForbiddenRecursionDepth > 0; }
#   else
    bool IsSlotAssignmentRecalcForbidden(void) { return false; }
#   endif

    constexpr bool Printing(void) { return false; }              // Just a placeholder.
    constexpr bool FinishingOrIsPrinting(void) { return false; } // Just a placeholder.
#endif // BLINKIT_CRAWLER_ONLY
protected:
    Document(const DocumentInit &initializer);
private:
    friend class IgnoreDestructiveWriteCountIncrementer;
    friend class ThrowOnDynamicMarkupInsertionCountIncrementer;
    friend class IgnoreOpensDuringUnloadCountIncrementer;

    static ConstructionType GetConstructionType(const DocumentInit &init);

    void DispatchDidReceiveTitle(void);
    void UpdateTitle(const String &title);
    void UpdateBaseURL(void);

    // ImplicitClose() actually does the work of closing the input stream.
    void ImplicitClose(void);
    bool ShouldComplete(void);
    // Returns |true| if both document and its owning frame are still attached.
    // Any of them could be detached during the check, e.g. by calling
    // iframe.remove() from an event handler.
    bool CheckCompletedInternal(void);
    void DetachParser(void);

    bool HaveImportsLoaded(void) const;
    bool HaveScriptBlockingStylesheetsLoaded(void) const;

    void LoadEventDelayTimerFired(TimerBase *);

    // EventTarget overrides
    void RemoveAllEventListeners(void) final;
    // Node overrides
    String nodeName(void) const final;
    NodeType getNodeType(void) const final { return kDocumentNode; }
    Node* Clone(Document &factory, CloneChildrenFlag flag) const override;
    bool ChildTypeAllowed(NodeType type) const final;
    // ContainerNode overrides
    void ChildrenChanged(const ChildrenChange &change) override;
    // ExecutionContext overrides
    bool IsDocument(void) const final { return true; }

#ifndef BLINKIT_CRAWLER_ONLY
    void ClearFocusedElementSoon(void);

    void BeginLifecycleUpdatesIfRenderingReady(void);

    bool NeedsFullLayoutTreeUpdate(void) const;
    bool ShouldScheduleLayoutTreeUpdate(void) const;
    void ScheduleLayoutTreeUpdate(void);

    void UpdateStyleInvalidationIfNeeded(void);
    void UpdateStyle(void);
    void NotifyLayoutTreeOfSubtreeChanges(void);

    void EvaluateMediaQueryListIfNeeded(void);
    void UpdateUseShadowTreesIfNeeded(void);

    bool HasPendingVisualUpdate(void) const { return m_lifecycle.GetState() == DocumentLifecycle::kVisualUpdatePending; }
    bool HaveRenderBlockingResourcesLoaded(void) const;

    void PropagateStyleToViewport(void);
    void ViewportDefiningElementDidChange(void);

    void AttachLayoutTree(AttachContext &) override { NOTREACHED(); }
    void DetachLayoutTree(const AttachContext & = AttachContext()) override { NOTREACHED(); }
#endif

    static uint64_t m_globalTreeVersion;
    uint64_t m_domTreeVersion;

    DocumentLifecycle m_lifecycle;
    Member<LocalFrame> m_frame;
    Member<LocalDOMWindow> m_domWindow;
    DocumentEncodingData m_encodingData;
    DocumentReadyState m_readyState = kComplete;
    ParsingState m_parsingState = kFinishedParsing;
    ShadowCascadeOrder m_shadowCascadeOrder = kShadowCascadeNone;

    unsigned short m_listenerTypes = 0;
    MutationObserverOptions m_mutationObserverTypes = 0;

    // https://html.spec.whatwg.org/C/dynamic-markup-insertion.html#ignore-destructive-writes-counter
    unsigned ignore_destructive_write_count_ = 0;
    // https://html.spec.whatwg.org/C/dynamic-markup-insertion.html#throw-on-dynamic-markup-insertion-counter
    unsigned throw_on_dynamic_markup_insertion_count_ = 0;
    // https://html.spec.whatwg.org/C/dynamic-markup-insertion.html#ignore-opens-during-unload-counter
    unsigned ignore_opens_during_unload_count_ = 0;

    bool m_wellFormed = false;

    String m_title, m_rawTitle;

    // Document URLs.
    GURL m_URL;  // Document.URL: The URL from which this document was retrieved.
    GURL m_baseURL;  // Node.baseURI: The URL to use when resolving relative URLs.
    GURL m_baseURLOverride;
    GURL m_baseElementURL;  // The URL set by the <base> element.
    GURL m_cookieURL;       // The URL to use for cookie access.

    AtomicString m_contentLanguage;

    Member<DocumentType> m_docType;
    Member<Element> m_titleElement;
    Member<Element> m_documentElement;

    CompatibilityMode m_compatibilityMode = kNoQuirksMode;
    bool m_compatibilityModeLocked = false;

    bool m_wasDiscarded = false;

    LoadEventProgress m_loadEventProgress = kLoadEventCompleted;
    int m_loadEventDelayCount = 0;
    TaskRunnerTimer<Document> m_loadEventDelayTimer;

    Member<ResourceFetcher> m_fetcher;
    Member<DocumentParser> m_parser;
    std::unique_ptr<ScriptRunner> m_scriptRunner;

    Member<ElementDataCache> m_elementDataCache;
    std::unique_ptr<SelectorQueryCache> m_selectorQueryCache;

    // It is safe to keep a raw, untraced pointer to this stack-allocated
    // cache object: it is set upon the cache object being allocated on
    // the stack and cleared upon leaving its allocated scope. Hence it
    // is acceptable not to trace it -- should a conservative GC occur,
    // the cache object's references will be traced by a stack walk.
    NthIndexCache *m_nthIndexCache = nullptr;

    int m_nodeCount = 0;
    std::unordered_set<const LiveNodeListBase *> m_listsInvalidatedAtDocument;
    LiveNodeListRegistry m_nodeLists;

    // Temporary flag for some UseCounter items. crbug.com/859391.
    InDOMNodeRemovedHandlerState m_inDomNodeRemovedHandlerState = InDOMNodeRemovedHandlerState::kNone;
    bool m_mayContainV0Shadow = false; // BKTODO: This may be useless for crawlers.

    TaskRunnerTimer<Document> m_elementDataCacheClearTimer;
    void ElementDataCacheClearTimerFired(TimerBase *);

    bool m_writeRecursionIsTooDeep = false;
    unsigned m_writeRecursionDepth = 0;

    std::stack<ScriptElementBase *> m_currentScriptStack;

#ifndef BLINKIT_CRAWLER_ONLY
    // If we do ignore the pending stylesheet count, then we need to add a boolean
    // to track that this happened so that we can do a full repaint when the
    // stylesheets do eventually load.
    PendingSheetLayout m_pendingSheetLayout = kNoLayoutWithPendingSheets;

    std::shared_ptr<HTMLImportsController> m_importsController;

    LayoutView *m_layoutView = nullptr;
    std::unique_ptr<TextAutosizer> m_textAutosizer;
    Member<Element> m_autofocusElement;
    Member<Element> m_focusedElement;
    Member<Element> m_hoverElement;
    Member<Element> m_activeElement;
    UserActionElementSet m_userActionElements;
    std::unique_ptr<RootScrollerController> m_rootScrollerController;
    Member<Document> m_templateDocumentHost;

    Member<CSSStyleSheet> m_elemSheet;
    Member<StyleEngine> m_styleEngine;
    Member<MediaQueryMatcher> m_mediaQueryMatcher;

    bool m_hasNodesWithPlaceholderStyle = false;
    bool m_evaluateMediaQueriesOnStyleRecalc = false;
    bool m_hasViewportUnits = false;

    std::shared_ptr<V0CustomElementRegistrationContext> m_registrationContext;

    Member<DocumentTimeline> m_timeline;

    TextLinkColors m_textLinkColors;
    const std::unique_ptr<VisitedLinkState> m_visitedLinkState;
    bool m_visuallyOrdered = false;

    // TODO(tkent): Should it be moved to LocalFrame or LocalFrameView?
    std::unique_ptr<ViewportData> m_viewportData;

    Member<SnapCoordinator> m_snapCoordinator;

    friend class ReattachLegacyLayoutObjectList;
    // TODO(layout-dev): Once everything are LayoutNG, we can get rid of this.
    // Used for legacy layout tree fallback
    ReattachLegacyLayoutObjectList *reattach_legacy_object_list_ = nullptr;

#   if DCHECK_IS_ON()
    unsigned m_slotAssignmentRecalcForbiddenRecursionDepth = 0;
#   endif
#endif
};

DEFINE_COMPARISON_OPERATORS_WITH_REFERENCES(Document)

template <>
struct DowncastTraits<Document> {
    static bool AllowFrom(const ExecutionContext &context)
    {
        return context.IsDocument();
    }
    static bool AllowFrom(const Node &node)
    {
        return node.IsDocumentNode();
    }
};

} // namespace blink

#endif // BLINKIT_BLINK_DOCUMENT_H
