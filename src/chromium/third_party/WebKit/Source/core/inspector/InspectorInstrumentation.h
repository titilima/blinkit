// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: InspectorInstrumentation.h
// Description: Placeholders for Inspector
//      Author: Ziming Li
//     Created: 2018-08-31
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INSPECTOR_INSTRUMENTATION_H
#define BLINKIT_BLINK_INSPECTOR_INSTRUMENTATION_H

#pragma once

#include "core/css/CSSSelector.h"
#include "core/page/ChromeClient.h"

namespace WTF {
class String;
}

namespace blink {

struct FetchInitiatorInfo;
class CharacterData;
class Document;
class DocumentLoader;
class Element;
class Event;
class EventTarget;
class ExecutionContext;
class ExecutionContextTask;
class GraphicsContext;
class GraphicsLayer;
class LayoutObject;
class LayoutRect;
class LocalFrame;
class Node;
class PseudoElement;
class ResourceLoader;
class ResourceResponse;
class QualifiedName;
class Resource;
class ResourceError;
class ShadowRoot;
class ThreadableLoaderClient;

typedef int InspectorInstrumentationCookie;

namespace InspectorInstrumentation {

inline void activeStyleSheetsUpdated(Document *) {}
inline void animationPlayStateChanged(Document *, Animation *, Animation::AnimationPlayState, Animation::AnimationPlayState) {}
inline void applyUserAgentOverride(LocalFrame *, String *) {}
inline void characterDataModified(CharacterData *) {}
inline bool collectingHTMLParseErrors(Document *) { return false; }
inline void continueAfterXFrameOptionsDenied(LocalFrame *, DocumentLoader *, unsigned long, const ResourceResponse &) {}
inline void continueWithPolicyIgnore(LocalFrame *, DocumentLoader *, unsigned long, const ResourceResponse &) {}
inline void didBlockRequest(LocalFrame *, const ResourceRequest &, DocumentLoader *, const FetchInitiatorInfo &, ResourceRequestBlockedReason) {}
inline void didCancelAnimationFrame(ExecutionContext *, int) {}
inline void didClearDocumentOfWindowObject(LocalFrame *) {}
inline void didCommitLoad(LocalFrame *, DocumentLoader *) {}
inline void didCreateAnimation(Document *, unsigned) {}
inline void didEnqueueEvent(EventTarget *, Event *) {}
inline void didFailLoading(LocalFrame *frame, unsigned long, const ResourceError &) {}
inline void didFinishEventSourceRequest(ExecutionContext *, ThreadableLoaderClient *) {}
inline void didFinishLoading(LocalFrame *, unsigned long, double, int64_t) {}
inline void didFireAnimationFrame(InspectorInstrumentationCookie) {}
inline void didInsertDOMNode(Node *) {}
inline void didInvalidateStyleAttr(Node *) {}
inline void didModifyDOMAttr(Element *, const QualifiedName &, const AtomicString &) {}
inline void didPaint(LayoutObject *, const GraphicsLayer *, GraphicsContext &, const LayoutRect &) {}
inline void didPerformExecutionContextTask(ExecutionContext *) {}
inline void didPerformElementShadowDistribution(Element *) {}
inline void didPostExecutionContextTask(ExecutionContext *, ExecutionContextTask *) {}
inline void didPushShadowRoot(Element *, ShadowRoot *) {}
inline void didRecalculateStyle(InspectorInstrumentationCookie, int) {}
inline void didReceiveCORSRedirectResponse(LocalFrame *, unsigned long, DocumentLoader *, const ResourceResponse &, ResourceLoader *) {}
inline void didReceiveData(LocalFrame *, unsigned long, const char *, int, int) {}
inline void didReceiveResourceResponse(LocalFrame *, unsigned long, DocumentLoader *, const ResourceResponse &, ResourceLoader *) {}
inline void didRemoveDOMAttr(Element *, const QualifiedName &) {}
inline void didRemoveEvent(EventTarget *, Event *) {}
inline void didRequestAnimationFrame(ExecutionContext *, int) {}
inline void didResizeMainFrame(LocalFrame *) {}
inline void didRunJavaScriptDialog(InspectorInstrumentationCookie, bool) {}
inline void didScheduleStyleRecalculation(Document *) {}
inline void didStartProvisionalLoad(LocalFrame *) {}
inline void didUpdateLayout(LocalFrame *) {}
inline void documentDetached(Document *) {}
inline void documentThreadableLoaderStartedLoadingForClient(ExecutionContext *, unsigned long, ThreadableLoaderClient *) {}
inline void domContentLoadedEventFired(LocalFrame *) {}
inline bool forcePseudoState(Element *, CSSSelector::PseudoType) { return false; }
inline void frameAttachedToParent(LocalFrame *) {}
inline void frameClearedScheduledNavigation(LocalFrame *) {}
inline void frameDetachedFromParent(LocalFrame *) {}
inline void frameScheduledNavigation(LocalFrame *, double) {}
inline void frameStartedLoading(LocalFrame *) {}
inline void frameStoppedLoading(LocalFrame *) {}
inline bool hasFrontends(void) { return false; }
inline bool isDebuggerPaused(LocalFrame *) { return false; }
inline void layerTreeDidChange(LocalFrame *) {}
inline void markResourceAsCached(LocalFrame *, unsigned long) {}
inline void mediaQueryResultChanged(Document *) {}
inline void networkStateChanged(LocalFrame *, bool) {}
inline void pseudoElementCreated(PseudoElement *) {}
inline void pseudoElementDestroyed(PseudoElement *) {}
inline void scriptExecutionBlockedByCSP(ExecutionContext *, const String &) {}
inline bool shouldForceCORSPreflight(Document *) { return false; }
inline void updateApplicationCacheStatus(LocalFrame *) {}
inline void willDestroyResource(Resource *) {}
inline void willDispachEventSourceEvent(ExecutionContext *, ThreadableLoaderClient *, const AtomicString &, const AtomicString &, const Vector<UChar> &) {}
inline InspectorInstrumentationCookie willFireAnimationFrame(ExecutionContext *, int) { return 0; }
inline void willInsertDOMNode(Node *) {}
inline void willModifyDOMAttr(Element *, const AtomicString &, const AtomicString &) {}
inline void willPerformExecutionContextTask(ExecutionContext *, ExecutionContextTask *) {}
inline void willPopShadowRoot(Element *, ShadowRoot *) {}
inline InspectorInstrumentationCookie willRecalculateStyle(Document *) { return 0; }
inline void willRemoveDOMNode(Node *) {}
inline InspectorInstrumentationCookie willRunJavaScriptDialog(LocalFrame *, const String &, ChromeClient::DialogType) { return 0; }
inline void willSendEventSourceRequest(ExecutionContext *, ThreadableLoaderClient *) {}
inline void willSendRequest(LocalFrame *, unsigned long, DocumentLoader *, ResourceRequest &, const ResourceResponse &, const FetchInitiatorInfo &) {}
inline void willSetInnerHTML(Element *) {}

} // namespace InspectorInstrumentation
} // namespace blink

#endif // BLINKIT_BLINK_INSPECTOR_INSTRUMENTATION_H
