// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: chrome_client_impl.h
// Description: ChromeClientImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_CHROME_CLIENT_IMPL_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_CHROME_CLIENT_IMPL_H_

#include <memory>
#if 0 // BKTODO:
#include "third_party/blink/public/web/web_navigation_policy.h"
#include "third_party/blink/public/web/web_window_features.h"
#endif
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/platform/graphics/touch_action.h"

namespace blink {

class PagePopup;
class PagePopupClient;
class WebAutofillClient;
struct WebCursorInfo;

// Handles window-level notifications from core on behalf of a WebView.
class CORE_EXPORT ChromeClientImpl final : public ChromeClient {
 public:
  static std::unique_ptr<ChromeClientImpl> Create(WebViewImpl*);
  ~ChromeClientImpl() override;

  WebViewImpl* GetWebView() const override;

  // ChromeClient methods:
  void ChromeDestroyed() override;
  void SetWindowRect(const IntRect&, LocalFrame&) override;
  IntRect RootWindowRect() override;
  IntRect PageRect() override;
  void Focus(LocalFrame*) override;
  bool CanTakeFocus(WebFocusType) override;
  void TakeFocus(WebFocusType) override;
  void FocusedNodeChanged(Node* from_node, Node* to_node) override;
  void BeginLifecycleUpdates() override;
  bool HadFormInteraction() const override;
#if 0 // BKTODO:
  void StartDragging(LocalFrame*,
                     const WebDragData&,
                     WebDragOperationsMask,
                     const SkBitmap& drag_image,
                     const WebPoint& drag_image_offset) override;
#endif
  bool AcceptsLoadDrops() const override;
#if 0 // BKTODO:
  Page* CreateWindow(LocalFrame*,
                     const FrameLoadRequest&,
                     const WebWindowFeatures&,
                     NavigationPolicy,
                     SandboxFlags) override;
#endif
  void Show(NavigationPolicy) override;
  void DidOverscroll(const FloatSize& overscroll_delta,
                     const FloatSize& accumulated_overscroll,
                     const FloatPoint& position_in_viewport,
                     const FloatSize& velocity_in_viewport,
                     const cc::OverscrollBehavior&) override;
  bool ShouldReportDetailedMessageForSource(LocalFrame&,
                                            const String&) override;
#if 0 // BKTODO:
  void AddMessageToConsole(LocalFrame*,
                           MessageSource,
                           MessageLevel,
                           const String& message,
                           unsigned line_number,
                           const String& source_id,
                           const String& stack_trace) override;
#endif
  bool CanOpenBeforeUnloadConfirmPanel() override;
  bool OpenBeforeUnloadConfirmPanelDelegate(LocalFrame*,
                                            bool is_reload) override;
  void CloseWindowSoon() override;
  bool OpenJavaScriptAlertDelegate(LocalFrame*, const String&) override;
  bool OpenJavaScriptConfirmDelegate(LocalFrame*, const String&) override;
  bool OpenJavaScriptPromptDelegate(LocalFrame*,
                                    const String& message,
                                    const String& default_value,
                                    String& result) override;
  bool TabsToLinks() override;
  void InvalidateRect(const IntRect&) override;
  void ScheduleAnimation(const LocalFrameView*) override;
  IntRect ViewportToScreen(const IntRect&,
                           const LocalFrameView*) const override;
  float WindowToViewportScalar(const float) const override;
#if 0 // BKTODO:
  WebScreenInfo GetScreenInfo() const override;
#endif
  base::Optional<IntRect> VisibleContentRectForPainting() const override;
  void ContentsSizeChanged(LocalFrame*, const IntSize&) const override;
  void PageScaleFactorChanged() const override;
  float ClampPageScaleFactorToLimits(float scale) const override;
  void MainFrameScrollOffsetChanged() const override;
  void ResizeAfterLayout() const override;
  void MainFrameLayoutUpdated() const override;
  void ShowMouseOverURL(const HitTestResult&) override;
  void SetToolTip(LocalFrame&, const String&, TextDirection) override;
  void DispatchViewportPropertiesDidChange(
      const ViewportDescription&) const override;
  void PrintDelegate(LocalFrame*) override;
#if 0 // BKTODO:
  ColorChooser* OpenColorChooser(LocalFrame*,
                                 ColorChooserClient*,
                                 const Color&) override;
#endif
  DateTimeChooser* OpenDateTimeChooser(
      DateTimeChooserClient*,
      const DateTimeChooserParameters&) override;
#if 0 // BKTODO:
  void OpenFileChooser(LocalFrame*, scoped_refptr<FileChooser>) override;
#endif
  void EnumerateChosenDirectory(FileChooser*) override;
  void SetCursor(const Cursor&, LocalFrame*) override;
  void SetCursorOverridden(bool) override;
  Cursor LastSetCursorForTesting() const override;
#if 0 // BKTODO:
  // The client keeps track of which touch/mousewheel event types have handlers,
  // and if they do, whether the handlers are passive and/or blocking. This
  // allows the client to know which optimizations can be used for the
  // associated event classes.
  void SetEventListenerProperties(LocalFrame*,
                                  cc::EventListenerClass,
                                  cc::EventListenerProperties) override;
  cc::EventListenerProperties EventListenerProperties(
      LocalFrame*,
      cc::EventListenerClass) const override;
#endif
  // Informs client about the existence of handlers for scroll events so
  // appropriate scroll optimizations can be chosen.
  void SetHasScrollEventHandlers(LocalFrame*, bool has_event_handlers) override;
  void SetNeedsLowLatencyInput(LocalFrame*, bool needs_low_latency) override;
  void RequestUnbufferedInputEvents(LocalFrame*) override;
  void SetTouchAction(LocalFrame*, TouchAction) override;

  void AttachRootGraphicsLayer(GraphicsLayer*, LocalFrame* local_root) override;

#if 0 // BKTODO:
  void AttachRootLayer(scoped_refptr<cc::Layer>,
                       LocalFrame* local_root) override;
#endif

  void AttachCompositorAnimationTimeline(CompositorAnimationTimeline*,
                                         LocalFrame*) override;
  void DetachCompositorAnimationTimeline(CompositorAnimationTimeline*,
                                         LocalFrame*) override;

  void EnterFullscreen(LocalFrame&, const FullscreenOptions&) override;
  void ExitFullscreen(LocalFrame&) override;
  void FullscreenElementChanged(Element* old_element,
                                Element* new_element) override;

  void ClearLayerSelection(LocalFrame*) override;
#if 0 // BKTODO:
  void UpdateLayerSelection(LocalFrame*, const cc::LayerSelection&) override;
#endif

  // ChromeClient methods:
  String AcceptLanguages() override;
  void SetCursorForPlugin(const WebCursorInfo&, LocalFrame*) override;

#if 0 // BKTODO:
  // ChromeClientImpl:
  void SetNewWindowNavigationPolicy(WebNavigationPolicy);
#endif

  // FileChooser calls this function to kick pending file chooser
  // requests.
  void DidCompleteFileChooser(FileChooser& file_chooser);

#if 0 // BKTODO:
  void AutoscrollStart(WebFloatPoint viewport_point, LocalFrame*) override;
  void AutoscrollFling(WebFloatSize velocity, LocalFrame*) override;
#endif
  void AutoscrollEnd(LocalFrame*) override;

  bool HasOpenedPopup() const override;
#if 0 // BKTODO:
  PopupMenu* OpenPopupMenu(LocalFrame&, HTMLSelectElement&) override;
#endif
  PagePopup* OpenPagePopup(PagePopupClient*) override;
  void ClosePagePopup(PagePopup*) override;
#if 0 // BKTODO:
  DOMWindow* PagePopupWindowForTesting() const override;
#endif

  void SetBrowserControlsState(float top_height,
                               float bottom_height,
                               bool shrinks_layout) override;
  void SetBrowserControlsShownRatio(float) override;

#if 0 // BKTODO:
  bool ShouldOpenModalDialogDuringPageDismissal(
      LocalFrame&,
      DialogType,
      const String& dialog_message,
      Document::PageDismissalType) const override;
#endif

  bool RequestPointerLock(LocalFrame*) override;
  void RequestPointerUnlock(LocalFrame*) override;

  // AutofillClient pass throughs:
  void DidAssociateFormControlsAfterLoad(LocalFrame*) override;
  void HandleKeyboardEventOnTextField(HTMLInputElement&,
                                      KeyboardEvent&) override;
  void DidChangeValueInTextField(HTMLFormControlElement&) override;
  void DidEndEditingOnTextField(HTMLInputElement&) override;
  void OpenTextDataListChooser(HTMLInputElement&) override;
  void TextFieldDataListChanged(HTMLInputElement&) override;
  void DidChangeSelectionInSelectControl(HTMLFormControlElement&) override;
  void SelectFieldOptionsChanged(HTMLFormControlElement&) override;
  void AjaxSucceeded(LocalFrame*) override;

  void ShowVirtualKeyboardOnElementFocus(LocalFrame&) override;

  void RegisterViewportLayers() const override;

  void OnMouseDown(Node&) override;
  void DidUpdateBrowserControls() const override;
  void SetOverscrollBehavior(const cc::OverscrollBehavior&) override;

  FloatSize ElasticOverscroll() const override;

  void RegisterPopupOpeningObserver(PopupOpeningObserver*) override;
  void UnregisterPopupOpeningObserver(PopupOpeningObserver*) override;
  void NotifyPopupOpeningObservers() const override;

  WebLayerTreeView* GetWebLayerTreeView(LocalFrame*) override;

#if 0 // BKTODO:
  void RequestDecode(LocalFrame*,
                     const PaintImage&,
                     base::OnceCallback<void(bool)>) override;
#endif

 private:
  explicit ChromeClientImpl(WebViewImpl*);

  bool IsChromeClientImpl() const override { return true; }

  void SetCursor(const WebCursorInfo&, LocalFrame*);

  // Returns WebAutofillClient associated with the WebLocalFrame. This takes and
  // returns nullable.
  WebAutofillClient* AutofillClientFromFrame(LocalFrame*);

  WebViewImpl* web_view_;  // Weak pointer.
  HeapHashSet<WeakMember<PopupOpeningObserver>> popup_opening_observers_;
  // BKTODO: Vector<scoped_refptr<FileChooser>> file_chooser_queue_;
  Cursor last_set_mouse_cursor_for_testing_;
  bool cursor_overridden_;
  bool did_request_non_empty_tool_tip_;

  FRIEND_TEST_ALL_PREFIXES(FileChooserQueueTest, DerefQueuedChooser);
};

DEFINE_TYPE_CASTS(ChromeClientImpl,
                  ChromeClient,
                  client,
                  client->IsChromeClientImpl(),
                  client.IsChromeClientImpl());

}  // namespace blink

#endif
