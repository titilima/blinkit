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

#include "third_party/blink/renderer/core/dom/container_node.h"
#include "third_party/blink/renderer/core/dom/create_element_flags.h"
#include "third_party/blink/renderer/core/dom/document_encoding_data.h"
#include "third_party/blink/renderer/core/dom/document_lifecycle.h"
#include "third_party/blink/renderer/core/dom/live_node_list_registry.h"
#include "third_party/blink/renderer/core/dom/mutation_observer_options.h"
#include "third_party/blink/renderer/core/dom/synchronous_mutation_notifier.h"
#include "third_party/blink/renderer/core/dom/synchronous_mutation_observer.h"
#include "third_party/blink/renderer/core/dom/tree_scope.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/timer.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "url/bk_url.h"

namespace blink {

class DocumentFragment;
class DocumentInit;
class DocumentLoader;
class DocumentParser;
class DocumentType;
class Element;
class ElementDataCache;
class ExceptionState;
class LayoutView;
class LocalDOMWindow;
class LocalFrame;
class ScriptableDocumentParser;

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
               , public SynchronousMutationNotifier
{
public:
    ~Document(void) override;

    void Initialize(void);
    virtual void Shutdown(void);

    uint64_t DomTreeVersion(void) const { return m_domTreeVersion; }
    void IncDOMTreeVersion(void)
    {
        ASSERT(m_lifecycle.StateAllowsTreeMutations());
        m_domTreeVersion = ++m_globalTreeVersion;
    }

    LocalDOMWindow* domWindow(void) const { return m_domWindow; }
    LocalFrame* GetFrame(void) const { return m_frame; }  // can be null

    void SetDoctype(DocumentType *docType);

    // Return the document URL, or an empty URL if it's unavailable.
    // This is not an implementation of web-exposed Document.prototype.URL.
    const BlinKit::BkURL& Url(void) const { return m_URL; }
    BlinKit::BkURL ValidBaseElementURL(void) const;

    Element* documentElement(void) const { return m_documentElement.Get(); }

    virtual Element* CreateElement(const AtomicString &name, CreateElementFlags flags) = 0;

    bool HasMutationObserversOfType(MutationType type) const { return 0 != (m_mutationObserverTypes & type); }

    int NodeCount(void) const { return m_nodeCount; }
    void IncrementNodeCount(void) { ++m_nodeCount; }
    void DecrementNodeCount(void)
    {
        ASSERT(m_nodeCount > 0);
        --m_nodeCount;
    }

    bool MayContainV0Shadow(void) const { return m_mayContainV0Shadow; }

    bool ShouldInvalidateNodeListCaches(const QualifiedName *attrName = nullptr) const;

    DocumentLoader* Loader(void) const;

    enum CompatibilityMode { kQuirksMode, kLimitedQuirksMode, kNoQuirksMode };
    CompatibilityMode GetCompatibilityMode(void) const { return m_compatibilityMode; }
    bool InQuirksMode(void) const { return kQuirksMode == m_compatibilityMode; }
    void SetCompatibilityMode(CompatibilityMode mode);

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
    void SuppressLoadEvent(void);

    DocumentParser* Parser(void) const { return m_parser.get(); }
    ScriptableDocumentParser* GetScriptableDocumentParser(void) const;
    std::shared_ptr<DocumentParser> OpenForNavigation(const AtomicString &mimeType, const AtomicString &encoding);
    std::shared_ptr<DocumentParser> ImplicitOpen(void);
    std::shared_ptr<DocumentParser> CreateParser(void);

    enum HttpRefreshType { kHttpRefreshFromHeader, kHttpRefreshFromMetaTag };
    void MaybeHandleHttpRefresh(const String &content, HttpRefreshType refreshType);

    bool IsScriptExecutionReady(void) const {
        ASSERT(false); // BKTODO:
        return false;
    }

    bool CanAcceptChild(const Node &newChild, const Node *next, const Node *oldChild, ExceptionState &exceptionState) const;

    bool ContainsV1ShadowTree(void) const { return ShadowCascadeOrder::kShadowCascadeV1 == m_shadowCascadeOrder; }

    // ExecutionContext overrides
    bool CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) override;
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType type) override;
protected:
    Document(const DocumentInit &initializer);
private:
    friend class IgnoreDestructiveWriteCountIncrementer;
    friend class ThrowOnDynamicMarkupInsertionCountIncrementer;

    static ConstructionType GetConstructionType(const DocumentInit &init);

    void DispatchDidReceiveTitle(void);

    void DetachParser(void);

    // Node overrides
    String nodeName(void) const final;
    NodeType getNodeType(void) const final { return kDocumentNode; }
    Node* Clone(Document &factory, CloneChildrenFlag flag) const override;
    // ContainerNode overrides
    void ChildrenChanged(const ChildrenChange &change) override;
    // ExecutionContext overrides
    bool IsDocument(void) const final { return true; }

    static uint64_t m_globalTreeVersion;
    uint64_t m_domTreeVersion;

    DocumentLifecycle m_lifecycle;
    Member<LocalDOMWindow> m_domWindow;
    Member<LocalFrame> m_frame;
    DocumentEncodingData m_encodingData;
    DocumentReadyState m_readyState = kComplete;
    ParsingState m_parsingState = kFinishedParsing;
    ShadowCascadeOrder m_shadowCascadeOrder = kShadowCascadeNone;

    MutationObserverOptions m_mutationObserverTypes = 0;

    unsigned ignore_destructive_write_count_ = 0;
    unsigned throw_on_dynamic_markup_insertion_count_ = 0;

    bool m_wellFormed = false;

    String m_title;

    // Document URLs.
    BlinKit::BkURL m_URL;  // Document.URL: The URL from which this document was retrieved.

    Member<Element> m_titleElement;
    Member<Element> m_documentElement;

    CompatibilityMode m_compatibilityMode = kNoQuirksMode;
    bool m_compatibilityModeLocked = false;

    bool m_wasDiscarded = false;

    LoadEventProgress m_loadEventProgress = kLoadEventCompleted;

    std::shared_ptr<DocumentParser> m_parser;
#ifndef BLINKIT_CRAWLER_ONLY
    LayoutView *m_layoutView = nullptr;
    Member<Element> m_focusedElement;
#endif

    std::unique_ptr<ElementDataCache> m_elementDataCache;

    int m_nodeCount = 0;
    LiveNodeListRegistry m_nodeLists;

    bool m_mayContainV0Shadow = false; // BKTODO: This may be useless for crawlers.

    TaskRunnerTimer<Document> m_elementDataCacheClearTimer;
    void ElementDataCacheClearTimerFired(TimerBase *);
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
