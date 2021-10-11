#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: InspectorInstrumentation.h
// Description: Placeholders for Inspector
//      Author: Ziming Li
//     Created: 2018-08-31
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_INSPECTOR_INSTRUMENTATION_H
#define BLINKIT_BLINKIT_INSPECTOR_INSTRUMENTATION_H

namespace blink {

using InspectorInstrumentationCookie = int;

namespace InspectorInstrumentation {

inline void DoNothing(void) {}
inline int DoNothingAndReturnInt(void) { return 0; }

inline bool collectingHTMLParseErrors(...) { return false; }
inline void didInvalidateStyleAttr(...) {}
inline void documentDetached(...) {}
inline bool forcePseudoState(...) { return false; }
inline bool hasFrontends(...) { return false; }
inline bool isDebuggerPaused(...) { return false; }

} // namespace InspectorInstrumentation

} // namespace blink

#define activeStyleSheetsUpdated(...)               DoNothing()
#define animationPlayStateChanged(...)              DoNothing()
#define applyUserAgentOverride(...)                 DoNothing()
#define characterDataModified(...)                  DoNothing()
#define didCancelAnimationFrame(...)                DoNothing()
#define didClearAllMutationRecords(...)             DoNothing()
#define didCreateAnimation(...)                     DoNothing()
#define didDeliverMutationRecords(...)              DoNothing()
#define didEnqueueEvent(...)                        DoNothing()
#define didEnqueueMutationRecord(...)               DoNothing()
#define didFireAnimationFrame(...)                  DoNothing()
#define didHandleEvent(...)                         DoNothing()
#define didInsertDOMNode(...)                       DoNothing()
#define didModifyDOMAttr(...)                       DoNothing()
#define didPaint(...)                               DoNothing()
#define didPerformElementShadowDistribution(...)    DoNothing()
#define didPerformExecutionContextTask(...)         DoNothing()
#define didPostExecutionContextTask(...)            DoNothing()
#define didPushShadowRoot(...)                      DoNothing()
#define didRecalculateStyle(...)                    DoNothing()
#define didRemoveDOMAttr(...)                       DoNothing()
#define didRemoveEvent(...)                         DoNothing()
#define didRequestAnimationFrame(...)               DoNothing()
#define didResizeMainFrame(...)                     DoNothing()
#define didScheduleStyleRecalculation(...)          DoNothing()
#define didUpdateLayout(...)                        DoNothing()
#define domContentLoadedEventFired(...)             DoNothing()
#define frameDetachedFromParent(...)                DoNothing()
#define layerTreeDidChange(...)                     DoNothing()
#define loadEventFired(...)                         DoNothing()
#define mediaQueryResultChanged(...)                DoNothing()
#define pseudoElementCreated(...)                   DoNothing()
#define pseudoElementDestroyed(...)                 DoNothing()
#define willDeliverMutationRecords(...)             DoNothing()
#define willFireAnimationFrame(...)                 DoNothingAndReturnInt()
#define willHandleEvent(...)                        DoNothingAndReturnInt()
#define willInsertDOMNode(...)                      DoNothing()
#define willModifyDOMAttr(...)                      DoNothing()
#define willPerformExecutionContextTask(...)        DoNothing()
#define willRecalculateStyle(...)                   DoNothingAndReturnInt()
#define willRemoveDOMNode(...)                      DoNothing()
#define willSetInnerHTML(...)                       DoNothing()

#endif // BLINKIT_BLINKIT_INSPECTOR_INSTRUMENTATION_H
