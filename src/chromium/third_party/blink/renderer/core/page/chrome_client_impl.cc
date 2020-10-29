// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: chrome_client_impl.cc
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

#include "third_party/blink/renderer/core/page/chrome_client_impl.h"

#include <memory>
#include <utility>

#include "base/optional.h"
#include "blinkit/ui/web_view_impl.h"
#include "build/build_config.h"
// BKTODO: #include "cc/layers/picture_layer.h"
#include "third_party/blink/public/platform/web_cursor_info.h"
#include "third_party/blink/public/platform/web_float_rect.h"
#include "third_party/blink/public/platform/web_rect.h"
// BKTODO: #include "third_party/blink/public/platform/web_url_request.h"
#include "third_party/blink/public/web/blink.h"
#if 0 // BKTODO:
#include "third_party/blink/public/web/web_autofill_client.h"
#include "third_party/blink/public/web/web_console_message.h"
#include "third_party/blink/public/web/web_input_element.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/public/web/web_node.h"
#include "third_party/blink/public/web/web_plugin.h"
#include "third_party/blink/public/web/web_popup_menu_info.h"
#include "third_party/blink/public/web/web_settings.h"
#include "third_party/blink/public/web/web_text_direction.h"
#include "third_party/blink/public/web/web_view_client.h"
#include "third_party/blink/public/web/web_window_features.h"
#include "third_party/blink/renderer/bindings/core/v8/script_controller.h"
#endif
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/node.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/events/web_input_event_conversion.h"
#include "third_party/blink/renderer/core/exported/web_plugin_container_impl.h"
#include "third_party/blink/renderer/core/exported/web_remote_frame_impl.h"
#include "third_party/blink/renderer/core/exported/web_settings_impl.h"
#include "third_party/blink/renderer/core/frame/browser_controls.h"
#endif
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/frame/web_frame_widget_base.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/core/fullscreen/fullscreen.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser_client.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser_popup_ui_controller.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser_ui_controller.h"
#include "third_party/blink/renderer/core/html/forms/date_time_chooser.h"
#include "third_party/blink/renderer/core/html/forms/date_time_chooser_client.h"
#include "third_party/blink/renderer/core/html/forms/date_time_chooser_impl.h"
#include "third_party/blink/renderer/core/html/forms/external_date_time_chooser.h"
#include "third_party/blink/renderer/core/html/forms/external_popup_menu.h"
#include "third_party/blink/renderer/core/html/forms/file_chooser.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/forms/internal_popup_menu.h"
#include "third_party/blink/renderer/core/inspector/dev_tools_emulator.h"
#endif
#include "third_party/blink/renderer/core/layout/hit_test_result.h"
// BKTODO: #include "third_party/blink/renderer/core/layout/layout_embedded_content.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/page/popup_opening_observer.h"
#include "third_party/blink/renderer/platform/animation/compositor_animation_host.h"
#endif
#include "third_party/blink/renderer/platform/cursor.h"
// BKTODO: #include "third_party/blink/renderer/platform/exported/wrapped_resource_request.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"
#include "third_party/blink/renderer/platform/graphics/touch_action.h"
#include "third_party/blink/renderer/platform/histogram.h"
// BKTODO: #include "third_party/blink/renderer/platform/layout_test_support.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
// BKTODO: #include "third_party/blink/renderer/platform/weborigin/security_origin.h"
#include "third_party/blink/renderer/platform/wtf/text/character_names.h"
#include "third_party/blink/renderer/platform/wtf/text/cstring.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"
#include "third_party/blink/renderer/platform/wtf/text/string_concatenate.h"

namespace blink {

namespace {

const char* DialogTypeToString(ChromeClient::DialogType dialog_type) {
  switch (dialog_type) {
    case ChromeClient::kAlertDialog:
      return "alert";
    case ChromeClient::kConfirmDialog:
      return "confirm";
    case ChromeClient::kPromptDialog:
      return "prompt";
    case ChromeClient::kPrintDialog:
      return "print";
    case ChromeClient::kHTMLDialog:
      NOTREACHED();
  }
  NOTREACHED();
  return "";
}

const char* DismissalTypeToString(Document::PageDismissalType dismissal_type) {
  switch (dismissal_type) {
    case Document::kBeforeUnloadDismissal:
      return "beforeunload";
    case Document::kPageHideDismissal:
      return "pagehide";
    case Document::kUnloadVisibilityChangeDismissal:
      return "visibilitychange";
    case Document::kUnloadDismissal:
      return "unload";
    case Document::kNoDismissal:
      NOTREACHED();
  }
  NOTREACHED();
  return "";
}

}  // namespace

class CompositorAnimationTimeline;

ChromeClientImpl::ChromeClientImpl(WebViewImpl* web_view)
    : web_view_(web_view),
      cursor_overridden_(false),
      did_request_non_empty_tool_tip_(false) {}

ChromeClientImpl::~ChromeClientImpl() {
  ASSERT(false); // BKTODO: DCHECK(file_chooser_queue_.IsEmpty());
}

std::unique_ptr<ChromeClientImpl> ChromeClientImpl::Create(WebViewImpl* web_view) {
  return base::WrapUnique(new ChromeClientImpl(web_view));
}

WebViewImpl* ChromeClientImpl::GetWebView() const {
  return web_view_;
}

void ChromeClientImpl::ChromeDestroyed() {
  // Our lifetime is bound to the WebViewImpl.
}

void ChromeClientImpl::SetWindowRect(const IntRect& r, LocalFrame& frame) {
  ASSERT(false); // BKTODO:
#if 0
  DCHECK_EQ(&frame, web_view_->MainFrameImpl()->GetFrame());
  WebWidgetClient* client =
      WebLocalFrameImpl::FromFrame(&frame)->FrameWidgetImpl()->Client();
  client->SetWindowRect(r);
#endif
}

IntRect ChromeClientImpl::RootWindowRect() {
  WebRect rect;
  ASSERT(false); // BKTODO:
#if 0
  if (web_view_->Client()) {
    rect = web_view_->Client()->RootWindowRect();
  } else {
    // These numbers will be fairly wrong. The window's x/y coordinates will
    // be the top left corner of the screen and the size will be the content
    // size instead of the window size.
    rect.width = web_view_->Size().width;
    rect.height = web_view_->Size().height;
  }
#endif
  return IntRect(rect);
}

IntRect ChromeClientImpl::PageRect() {
  // We hide the details of the window's border thickness from the web page by
  // simple re-using the window position here.  So, from the point-of-view of
  // the web page, the window has no border.
  return RootWindowRect();
}

void ChromeClientImpl::Focus(LocalFrame* calling_frame) {
  ASSERT(false); // BKTODO:
#if 0
  if (web_view_->Client()) {
    web_view_->Client()->DidFocus(
        calling_frame ? WebLocalFrameImpl::FromFrame(calling_frame) : nullptr);
  }
#endif
}

bool ChromeClientImpl::CanTakeFocus(WebFocusType) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  // For now the browser can always take focus if we're not running layout
  // tests.
  return !LayoutTestSupport::IsRunningLayoutTest();
#endif
}

void ChromeClientImpl::TakeFocus(WebFocusType type) {
  ASSERT(false); // BKTODO:
#if 0
  if (!web_view_->Client())
    return;
  if (type == kWebFocusTypeBackward)
    web_view_->Client()->FocusPrevious();
  else
    web_view_->Client()->FocusNext();
#endif
}

void ChromeClientImpl::FocusedNodeChanged(Node* from_node, Node* to_node) {
  ASSERT(false); // BKTODO:
#if 0
  if (!web_view_->Client())
    return;

  web_view_->Client()->FocusedNodeChanged(WebNode(from_node), WebNode(to_node));

  WebURL focus_url;
  if (to_node && to_node->IsElementNode() && ToElement(to_node)->IsLiveLink() &&
      to_node->ShouldHaveFocusAppearance())
    focus_url = ToElement(to_node)->HrefURL();
  web_view_->Client()->SetKeyboardFocusURL(focus_url);
#endif
}

bool ChromeClientImpl::HadFormInteraction() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return web_view_->PageImportanceSignals() &&
         web_view_->PageImportanceSignals()->HadFormInteraction();
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::StartDragging(LocalFrame* frame,
                                     const WebDragData& drag_data,
                                     WebDragOperationsMask mask,
                                     const SkBitmap& drag_image,
                                     const WebPoint& drag_image_offset) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebReferrerPolicy policy = web_frame->GetDocument().GetReferrerPolicy();
  web_frame->LocalRootFrameWidget()->StartDragging(
      policy, drag_data, mask, drag_image, drag_image_offset);
}
#endif

bool ChromeClientImpl::AcceptsLoadDrops() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return !web_view_->Client() || web_view_->Client()->AcceptsLoadDrops();
#endif
}

#if 0 // BKTODO:
Page* ChromeClientImpl::CreateWindow(LocalFrame* frame,
                                     const FrameLoadRequest& r,
                                     const WebWindowFeatures& features,
                                     NavigationPolicy navigation_policy,
                                     SandboxFlags sandbox_flags) {
  if (!web_view_->Client())
    return nullptr;

  if (!frame->GetPage() || frame->GetPage()->Paused())
    return nullptr;

  NotifyPopupOpeningObservers();
  const AtomicString& frame_name =
      !EqualIgnoringASCIICase(r.FrameName(), "_blank") ? r.FrameName()
                                                       : g_empty_atom;
  WebViewImpl* new_view =
      static_cast<WebViewImpl*>(web_view_->Client()->CreateView(
          WebLocalFrameImpl::FromFrame(frame),
          WrappedResourceRequest(r.GetResourceRequest()), features, frame_name,
          static_cast<WebNavigationPolicy>(navigation_policy),
          r.GetShouldSetOpener() == kNeverSetOpener,
          static_cast<WebSandboxFlags>(sandbox_flags)));
  if (!new_view)
    return nullptr;
  return new_view->GetPage();
}
#endif

void ChromeClientImpl::DidOverscroll(const FloatSize& overscroll_delta,
                                     const FloatSize& accumulated_overscroll,
                                     const FloatPoint& position_in_viewport,
                                     const FloatSize& velocity_in_viewport,
                                     const cc::OverscrollBehavior& behavior) {
  ASSERT(false); // BKTODO:
#if 0
  if (!web_view_->WidgetClient())
    return;

  web_view_->WidgetClient()->DidOverscroll(
      overscroll_delta, accumulated_overscroll, position_in_viewport,
      velocity_in_viewport, behavior);
#endif
}

void ChromeClientImpl::Show(NavigationPolicy navigation_policy) {
  ASSERT(false); // BKTODO:
#if 0
  if (web_view_->WidgetClient()) {
    web_view_->WidgetClient()->Show(
        static_cast<WebNavigationPolicy>(navigation_policy));
  }
#endif
}

bool ChromeClientImpl::ShouldReportDetailedMessageForSource(
    LocalFrame& local_frame,
    const String& url) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  WebLocalFrameImpl* webframe =
      WebLocalFrameImpl::FromFrame(&local_frame.LocalFrameRoot());
  return webframe && webframe->Client() &&
         webframe->Client()->ShouldReportDetailedMessageForSource(url);
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::AddMessageToConsole(LocalFrame* local_frame,
                                           MessageSource source,
                                           MessageLevel level,
                                           const String& message,
                                           unsigned line_number,
                                           const String& source_id,
                                           const String& stack_trace) {
  WebLocalFrameImpl* frame = WebLocalFrameImpl::FromFrame(local_frame);
  if (frame && frame->Client()) {
    frame->Client()->DidAddMessageToConsole(
        WebConsoleMessage(static_cast<WebConsoleMessage::Level>(level),
                          message),
        source_id, line_number, stack_trace);
  }
}
#endif

bool ChromeClientImpl::CanOpenBeforeUnloadConfirmPanel() {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return !!web_view_->Client();
#endif
}

bool ChromeClientImpl::OpenBeforeUnloadConfirmPanelDelegate(LocalFrame* frame,
                                                            bool is_reload) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO:
  return false;
#if 0
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  return webframe->Client() &&
         webframe->Client()->RunModalBeforeUnloadDialog(is_reload);
#endif
}

void ChromeClientImpl::CloseWindowSoon() {
  ASSERT(false); // BKTODO:
#if 0
  if (web_view_->WidgetClient())
    web_view_->WidgetClient()->CloseWidgetSoon();
#endif
}

// Although a LocalFrame is passed in, we don't actually use it, since we
// already know our own m_webView.
bool ChromeClientImpl::OpenJavaScriptAlertDelegate(LocalFrame* frame,
                                                   const String& message) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (UserGestureIndicator::ProcessingUserGesture())
      UserGestureIndicator::SetTimeoutPolicy(UserGestureToken::kHasPaused);
    webframe->Client()->RunModalAlertDialog(message);
    return true;
  }
#endif
  return false;
}

// See comments for openJavaScriptAlertDelegate().
bool ChromeClientImpl::OpenJavaScriptConfirmDelegate(LocalFrame* frame,
                                                     const String& message) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (UserGestureIndicator::ProcessingUserGesture())
      UserGestureIndicator::SetTimeoutPolicy(UserGestureToken::kHasPaused);
    return webframe->Client()->RunModalConfirmDialog(message);
  }
#endif
  return false;
}

// See comments for openJavaScriptAlertDelegate().
bool ChromeClientImpl::OpenJavaScriptPromptDelegate(LocalFrame* frame,
                                                    const String& message,
                                                    const String& default_value,
                                                    String& result) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (UserGestureIndicator::ProcessingUserGesture())
      UserGestureIndicator::SetTimeoutPolicy(UserGestureToken::kHasPaused);
    WebString actual_value;
    bool ok = webframe->Client()->RunModalPromptDialog(message, default_value,
                                                       &actual_value);
    if (ok)
      result = actual_value;
    return ok;
  }
#endif
  return false;
}
bool ChromeClientImpl::TabsToLinks() {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return web_view_->TabsToLinks();
#endif
}

void ChromeClientImpl::InvalidateRect(const IntRect& update_rect) {
  ASSERT(false); // BKTODO:
#if 0
  if (!update_rect.IsEmpty())
    web_view_->InvalidateRect(update_rect);
#endif
}

void ChromeClientImpl::ScheduleAnimation(const LocalFrameView* frame_view) {
  web_view_->ScheduleAnimation();
}

IntRect ChromeClientImpl::ViewportToScreen(
    const IntRect& rect_in_viewport,
    const LocalFrameView* frame_view) const {
  WebRect screen_rect(rect_in_viewport);

  ASSERT(false); // BKTODO:
#if 0
  LocalFrame& frame = frame_view->GetFrame();

  WebWidgetClient* client =
      WebLocalFrameImpl::FromFrame(&frame)->LocalRootFrameWidget()->Client();

  // TODO(dcheng): Is this null check needed?
  if (client) {
    client->ConvertViewportToWindow(&screen_rect);
    WebRect view_rect = client->ViewRect();
    screen_rect.x += view_rect.x;
    screen_rect.y += view_rect.y;
  }
#endif

  return screen_rect;
}

float ChromeClientImpl::WindowToViewportScalar(const float scalar_value) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  if (!web_view_->WidgetClient())
    return scalar_value;
  WebFloatRect viewport_rect(0, 0, scalar_value, 0);
  web_view_->WidgetClient()->ConvertWindowToViewport(&viewport_rect);
  return viewport_rect.width;
#endif
}

#if 0 // BKTODO:
WebScreenInfo ChromeClientImpl::GetScreenInfo() const {
  if (!web_view_->WidgetClient())
    return {};
  return web_view_->WidgetClient()->GetScreenInfo();
}
#endif

base::Optional<IntRect> ChromeClientImpl::VisibleContentRectForPainting()
    const {
  ASSERT(false); // BKTODO:
  return std::nullopt;
#if 0
  return web_view_->GetDevToolsEmulator()->VisibleContentRectForPainting();
#endif
}

void ChromeClientImpl::ContentsSizeChanged(LocalFrame* frame,
                                           const IntSize& size) const {
  ASSERT(false); // BKTODO:
#if 0
  web_view_->DidChangeContentsSize();

  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  webframe->DidChangeContentsSize(size);
#endif
}

void ChromeClientImpl::PageScaleFactorChanged() const {
  ASSERT(false); // BKTODO: web_view_->PageScaleFactorChanged();
}

void ChromeClientImpl::MainFrameScrollOffsetChanged() const {
  ASSERT(false); // BKTODO: web_view_->MainFrameScrollOffsetChanged();
}

float ChromeClientImpl::ClampPageScaleFactorToLimits(float scale) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return web_view_->ClampPageScaleFactorToLimits(scale);
#endif
}

void ChromeClientImpl::ResizeAfterLayout() const {
  ASSERT(false); // BKTODO: web_view_->ResizeAfterLayout();
}

void ChromeClientImpl::MainFrameLayoutUpdated() const {
  ASSERT(false); // BKTODO: web_view_->MainFrameLayoutUpdated();
}

void ChromeClientImpl::ShowMouseOverURL(const HitTestResult& result) {
  ASSERT(false); // BKTODO:
#if 0
  if (!web_view_->Client())
    return;

  WebURL url;

  // Ignore URL if hitTest include scrollbar since we might have both a
  // scrollbar and an element in the case of overlay scrollbars.
  if (!result.GetScrollbar()) {
    // Find out if the mouse is over a link, and if so, let our UI know...
    if (result.IsLiveLink() &&
        !result.AbsoluteLinkURL().GetString().IsEmpty()) {
      url = result.AbsoluteLinkURL();
    } else if (result.InnerNode() &&
               (IsHTMLObjectElement(*result.InnerNode()) ||
                IsHTMLEmbedElement(*result.InnerNode()))) {
      LayoutObject* object = result.InnerNode()->GetLayoutObject();
      if (object && object->IsLayoutEmbeddedContent()) {
        WebPluginContainerImpl* plugin_view =
            ToLayoutEmbeddedContent(object)->Plugin();
        if (plugin_view) {
          url = plugin_view->Plugin()->LinkAtPosition(
              result.RoundedPointInInnerNodeFrame());
        }
      }
    }
  }

  web_view_->Client()->SetMouseOverURL(url);
#endif
}

void ChromeClientImpl::SetToolTip(LocalFrame& frame,
                                  const String& tooltip_text,
                                  TextDirection dir) {
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(&frame);
  if (!tooltip_text.IsEmpty()) {
    web_frame->LocalRootFrameWidget()->Client()->SetToolTipText(
        tooltip_text, ToWebTextDirection(dir));
    did_request_non_empty_tool_tip_ = true;
  } else if (did_request_non_empty_tool_tip_) {
    // WebWidgetClient::setToolTipText will send an IPC message.  We'd like to
    // reduce the number of setToolTipText calls.
    web_frame->LocalRootFrameWidget()->Client()->SetToolTipText(
        tooltip_text, ToWebTextDirection(dir));
    did_request_non_empty_tool_tip_ = false;
  }
#endif
}

void ChromeClientImpl::DispatchViewportPropertiesDidChange(
    const ViewportDescription& description) const {
  ASSERT(false); // BKTODO: web_view_->UpdatePageDefinedViewportConstraints(description);
}

void ChromeClientImpl::PrintDelegate(LocalFrame* frame) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO: Is this necessary?
#if 0
  if (web_view_->Client())
    web_view_->Client()->PrintPage(WebLocalFrameImpl::FromFrame(frame));
#endif
}

#if 0 // BKTODO:
ColorChooser* ChromeClientImpl::OpenColorChooser(
    LocalFrame* frame,
    ColorChooserClient* chooser_client,
    const Color&) {
  NotifyPopupOpeningObservers();
  ColorChooserUIController* controller = nullptr;

  // TODO(crbug.com/779126): add support for the chooser in immersive mode.
  if (frame->GetDocument()->GetSettings()->GetImmersiveModeEnabled())
    return nullptr;

  if (RuntimeEnabledFeatures::PagePopupEnabled()) {
    controller =
        ColorChooserPopupUIController::Create(frame, this, chooser_client);
  } else {
    controller = ColorChooserUIController::Create(frame, chooser_client);
  }
  controller->OpenUI();
  return controller;
}
#endif

DateTimeChooser* ChromeClientImpl::OpenDateTimeChooser(
    DateTimeChooserClient* picker_client,
    const DateTimeChooserParameters& parameters) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  // TODO(crbug.com/779126): add support for the chooser in immersive mode.
  if (picker_client->OwnerElement()
          .GetDocument()
          .GetSettings()
          ->GetImmersiveModeEnabled())
    return nullptr;

  NotifyPopupOpeningObservers();
  if (RuntimeEnabledFeatures::InputMultipleFieldsUIEnabled())
    return DateTimeChooserImpl::Create(this, picker_client, parameters);
  return ExternalDateTimeChooser::Create(this, web_view_->Client(),
                                         picker_client, parameters);
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::OpenFileChooser(
    LocalFrame* frame,
    scoped_refptr<FileChooser> file_chooser) {
  NotifyPopupOpeningObservers();

  Document* doc = frame->GetDocument();
  if (doc)
    doc->MaybeQueueSendDidEditFieldInInsecureContext();

  static const wtf_size_t kMaximumPendingFileChooseRequests = 4;
  if (file_chooser_queue_.size() > kMaximumPendingFileChooseRequests) {
    // This sanity check prevents too many file choose requests from getting
    // queued which could DoS the user. Getting these is most likely a
    // programming error (there are many ways to DoS the user so it's not
    // considered a "real" security check), either in JS requesting many file
    // choosers to pop up, or in a plugin.
    //
    // TODO(brettw): We might possibly want to require a user gesture to open
    // a file picker, which will address this issue in a better way.
    return;
  }
  file_chooser_queue_.push_back(file_chooser.get());
  if (file_chooser_queue_.size() == 1) {
    // Actually show the browse dialog when this is the first request.
    if (file_chooser->OpenFileChooser(*this))
      return;
    // Choosing failed, so try the next chooser.
    DidCompleteFileChooser(*file_chooser);
  }
}
#endif

void ChromeClientImpl::DidCompleteFileChooser(FileChooser& chooser) {
  ASSERT(false); // BKTODO:
#if 0
  if (!file_chooser_queue_.IsEmpty() &&
      file_chooser_queue_.front().get() != &chooser) {
    // This function is called even if |chooser| wasn't stored in
    // file_chooser_queue_.
    return;
  }
  file_chooser_queue_.EraseAt(0);
  if (file_chooser_queue_.IsEmpty())
    return;
  FileChooser* next_chooser = file_chooser_queue_.front().get();
  if (next_chooser->OpenFileChooser(*this))
    return;
  // Choosing failed, so try the next chooser.
  DidCompleteFileChooser(*next_chooser);
#endif
}

void ChromeClientImpl::EnumerateChosenDirectory(FileChooser* file_chooser) {
  ASSERT(false); // BKTODO:
#if 0
  WebViewClient* client = web_view_->Client();
  if (!client)
    return;

  DCHECK(file_chooser);
  DCHECK(file_chooser->Params().selected_files.size());
  if (client->EnumerateChosenDirectory(file_chooser->Params().selected_files[0],
                                       file_chooser))
    file_chooser->AddRef();
#endif
}

Cursor ChromeClientImpl::LastSetCursorForTesting() const {
  return last_set_mouse_cursor_for_testing_;
}

void ChromeClientImpl::SetCursor(const Cursor& cursor,
                                 LocalFrame* local_frame) {
  last_set_mouse_cursor_for_testing_ = cursor;
  SetCursor(WebCursorInfo(cursor), local_frame);
}

void ChromeClientImpl::SetCursor(const WebCursorInfo& cursor,
                                 LocalFrame* local_frame) {
  if (cursor_overridden_)
    return;

#if defined(OS_MACOSX)
  // On Mac the mousemove event propagates to both the popup and main window.
  // If a popup is open we don't want the main window to change the cursor.
  if (web_view_->HasOpenedPopup())
    return;
#endif

  ASSERT(false); // BKTODO:
#if 0
  // TODO(dcheng): Why is this null check necessary?
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(local_frame)->LocalRootFrameWidget())
    widget->Client()->DidChangeCursor(cursor);
#endif
}

void ChromeClientImpl::SetCursorForPlugin(const WebCursorInfo& cursor,
                                          LocalFrame* local_frame) {
  SetCursor(cursor, local_frame);
}

void ChromeClientImpl::SetCursorOverridden(bool overridden) {
  cursor_overridden_ = overridden;
}

#if 0 // BKTODO:
void ChromeClientImpl::AutoscrollStart(WebFloatPoint viewport_point,
                                       LocalFrame* local_frame) {
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(local_frame)->LocalRootFrameWidget())
    widget->Client()->AutoscrollStart(viewport_point);
}

void ChromeClientImpl::AutoscrollFling(WebFloatSize velocity,
                                       LocalFrame* local_frame) {
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(local_frame)->LocalRootFrameWidget())
    widget->Client()->AutoscrollFling(velocity);
}
#endif

void ChromeClientImpl::AutoscrollEnd(LocalFrame* local_frame) {
  ASSERT(false); // BKTODO:
#if 0
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(local_frame)->LocalRootFrameWidget())
    widget->Client()->AutoscrollEnd();
#endif
}

String ChromeClientImpl::AcceptLanguages() {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return web_view_->Client()->AcceptLanguages();
#endif
}

void ChromeClientImpl::AttachRootGraphicsLayer(GraphicsLayer* root_layer,
                                               LocalFrame* local_frame) {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
  ASSERT(false); // BKTODO:
#if 0
  // TODO(dcheng): This seems wrong. Non-local roots shouldn't be calling this
  // function.
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(local_frame)->LocalRoot();
  DCHECK(WebLocalFrameImpl::FromFrame(local_frame) == web_frame);

  // This method can be called while the frame is being detached. In that
  // case, the rootLayer is null, and the widget is already destroyed.
  // TODO(dcheng): This should be called before the widget is gone...
  DCHECK(web_frame->FrameWidgetImpl() || !root_layer);
  if (web_frame->FrameWidgetImpl())
    web_frame->FrameWidgetImpl()->SetRootGraphicsLayer(root_layer);
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::AttachRootLayer(scoped_refptr<cc::Layer> root_layer,
                                       LocalFrame* local_frame) {
  // TODO(dcheng): This seems wrong. Non-local roots shouldn't be calling this
  // function.
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(local_frame)->LocalRoot();
  DCHECK(WebLocalFrameImpl::FromFrame(local_frame) == web_frame);

  // This method can be called while the frame is being detached. In that
  // case, the rootLayer is null, and the widget is already destroyed.
  // TODO(dcheng): This should be called before the widget is gone...
  DCHECK(web_frame->FrameWidget() || !root_layer);
  if (web_frame->FrameWidgetImpl())
    web_frame->FrameWidgetImpl()->SetRootLayer(std::move(root_layer));
}
#endif

void ChromeClientImpl::AttachCompositorAnimationTimeline(
    CompositorAnimationTimeline* compositor_timeline,
    LocalFrame* local_frame) {
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(local_frame);
  if (CompositorAnimationHost* animation_host =
          web_frame->LocalRootFrameWidget()->AnimationHost())
    animation_host->AddTimeline(*compositor_timeline);
#endif
}

void ChromeClientImpl::DetachCompositorAnimationTimeline(
    CompositorAnimationTimeline* compositor_timeline,
    LocalFrame* local_frame) {
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(local_frame);

  // This method can be called when the frame is being detached, after the
  // widget is destroyed.
  // TODO(dcheng): This should be called before the widget is gone...
  if (web_frame->LocalRootFrameWidget()) {
    if (CompositorAnimationHost* animation_host =
            web_frame->LocalRootFrameWidget()->AnimationHost())
      animation_host->RemoveTimeline(*compositor_timeline);
  }
#endif
}

void ChromeClientImpl::EnterFullscreen(LocalFrame& frame,
                                       const FullscreenOptions& options) {
  ASSERT(false); // BKTODO: web_view_->EnterFullscreen(frame, options);
}

void ChromeClientImpl::ExitFullscreen(LocalFrame& frame) {
  ASSERT(false); // BKTODO: web_view_->ExitFullscreen(frame);
}

void ChromeClientImpl::FullscreenElementChanged(Element* old_element,
                                                Element* new_element) {
  ASSERT(false); // BKTODO: web_view_->FullscreenElementChanged(old_element, new_element);
}

void ChromeClientImpl::ClearLayerSelection(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
#if 0
  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRootFrameWidget();
  WebWidgetClient* client = widget->Client();
  // TODO(dcheng): This shouldn't be called on detached frames?
  if (!client)
    return;

  if (WebLayerTreeView* layer_tree_view = widget->GetLayerTreeView())
    layer_tree_view->ClearSelection();
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::UpdateLayerSelection(
    LocalFrame* frame,
    const cc::LayerSelection& selection) {
  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRootFrameWidget();
  WebWidgetClient* client = widget->Client();
  // TODO(dcheng): This shouldn't be called on detached frames?
  if (!client)
    return;

  if (WebLayerTreeView* layer_tree_view = widget->GetLayerTreeView())
    layer_tree_view->RegisterSelection(selection);
}
#endif

bool ChromeClientImpl::HasOpenedPopup() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return web_view_->HasOpenedPopup();
#endif
}

#if 0 // BKTODO:
PopupMenu* ChromeClientImpl::OpenPopupMenu(LocalFrame& frame,
                                           HTMLSelectElement& select) {
  NotifyPopupOpeningObservers();
  if (WebViewImpl::UseExternalPopupMenus())
    return new ExternalPopupMenu(frame, select, *web_view_);

  DCHECK(RuntimeEnabledFeatures::PagePopupEnabled());
  return InternalPopupMenu::Create(this, select);
}
#endif

PagePopup* ChromeClientImpl::OpenPagePopup(PagePopupClient* client) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return web_view_->OpenPagePopup(client);
#endif
}

void ChromeClientImpl::ClosePagePopup(PagePopup* popup) {
  ASSERT(false); // BKTODO: web_view_->ClosePagePopup(popup);
}

#if 0 // BKTODO:
DOMWindow* ChromeClientImpl::PagePopupWindowForTesting() const {
  return web_view_->PagePopupWindow();
}
#endif

void ChromeClientImpl::SetBrowserControlsState(float top_height,
                                               float bottom_height,
                                               bool shrinks_layout) {
  ASSERT(false); // BKTODO:
#if 0
  WebSize size = web_view_->Size();
  if (shrinks_layout)
    size.height -= top_height + bottom_height;

  web_view_->ResizeWithBrowserControls(size, top_height, bottom_height,
                                       shrinks_layout);
#endif
}

void ChromeClientImpl::SetBrowserControlsShownRatio(float ratio) {
  ASSERT(false); // BKTODO: web_view_->GetBrowserControls().SetShownRatio(ratio);
}

#if 0 // BKTODO:
bool ChromeClientImpl::ShouldOpenModalDialogDuringPageDismissal(
    LocalFrame& frame,
    DialogType dialog_type,
    const String& dialog_message,
    Document::PageDismissalType dismissal_type) const {
  String message = String("Blocked ") + DialogTypeToString(dialog_type) + "('" +
                   dialog_message + "') during " +
                   DismissalTypeToString(dismissal_type) + ".";
  WebLocalFrameImpl::FromFrame(frame)->AddMessageToConsole(
      WebConsoleMessage(WebConsoleMessage::kLevelError, message));

  return false;
}
#endif

WebLayerTreeView* ChromeClientImpl::GetWebLayerTreeView(LocalFrame* frame) {
  CHECK(frame);
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  CHECK(web_frame);
  if (WebFrameWidgetBase* frame_widget = web_frame->LocalRootFrameWidget())
    return frame_widget->GetLayerTreeView();
#endif
  return nullptr;
}

#if 0 // BKTODO:
void ChromeClientImpl::RequestDecode(LocalFrame* frame,
                                     const PaintImage& image,
                                     base::OnceCallback<void(bool)> callback) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  web_frame->LocalRootFrameWidget()->RequestDecode(image, std::move(callback));
}

void ChromeClientImpl::SetEventListenerProperties(
    LocalFrame* frame,
    cc::EventListenerClass event_class,
    cc::EventListenerProperties properties) {
  // |frame| might be null if called via TreeScopeAdopter::
  // moveNodeToNewDocument() and the new document has no frame attached.
  // Since a document without a frame cannot attach one later, it is safe to
  // exit early.
  if (!frame)
    return;

  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  // The widget may be nullptr if the frame is provisional.
  // TODO(dcheng): This needs to be cleaned up at some point.
  // https://crbug.com/578349
  if (web_frame->IsProvisional()) {
    // If we hit a provisional frame, we expect it to be during initialization
    // in which case the |properties| should be 'nothing'.
    DCHECK(properties == cc::EventListenerProperties::kNone);
    return;
  }
  WebFrameWidgetBase* widget = web_frame->LocalRootFrameWidget();
  // TODO(https://crbug.com/820787): When creating a local root, the widget
  // won't be set yet. While notifications in this case are technically
  // redundant, it adds an awkward special case.
  if (!widget) {
    return;
  }

  // This relies on widget always pointing to a WebFrameWidgetBase when
  // |frame| points to an OOPIF frame, i.e. |frame|'s mainFrame() is
  // remote.
  WebWidgetClient* client = widget->Client();
  if (WebLayerTreeView* tree_view = widget->GetLayerTreeView()) {
    tree_view->SetEventListenerProperties(event_class, properties);
    if (event_class == cc::EventListenerClass::kTouchStartOrMove) {
      client->HasTouchEventHandlers(
          properties != cc::EventListenerProperties::kNone ||
          tree_view->EventListenerProperties(
              cc::EventListenerClass::kTouchEndOrCancel) !=
              cc::EventListenerProperties::kNone);
    } else if (event_class == cc::EventListenerClass::kTouchEndOrCancel) {
      client->HasTouchEventHandlers(
          properties != cc::EventListenerProperties::kNone ||
          tree_view->EventListenerProperties(
              cc::EventListenerClass::kTouchStartOrMove) !=
              cc::EventListenerProperties::kNone);
    } else if (event_class == cc::EventListenerClass::kPointerRawMove) {
      client->HasPointerRawMoveEventHandlers(
          properties != cc::EventListenerProperties::kNone);
    }
  } else {
    client->HasTouchEventHandlers(true);
  }
}
#endif

void ChromeClientImpl::BeginLifecycleUpdates() {
  ASSERT(false); // BKTODO:
#if 0
  web_view_->StopDeferringCommits();

  if (WebLayerTreeView* tree_view = web_view_->LayerTreeView()) {
    tree_view->SetNeedsBeginFrame();
  }
#endif
}

#if 0 // BKTODO:
cc::EventListenerProperties ChromeClientImpl::EventListenerProperties(
    LocalFrame* frame,
    cc::EventListenerClass event_class) const {
  if (!frame)
    return cc::EventListenerProperties::kNone;

  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRootFrameWidget();

  if (!widget || !widget->GetLayerTreeView())
    return cc::EventListenerProperties::kNone;
  return widget->GetLayerTreeView()->EventListenerProperties(event_class);
}
#endif

void ChromeClientImpl::SetHasScrollEventHandlers(LocalFrame* frame,
                                                 bool has_event_handlers) {
  // |frame| might be null if called via TreeScopeAdopter::
  // moveNodeToNewDocument() and the new document has no frame attached.
  // Since a document without a frame cannot attach one later, it is safe to
  // exit early.
  if (!frame)
    return;

  ASSERT(false); // BKTODO:
#if 0
  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRootFrameWidget();
  // While a frame is shutting down, we may get called after the layerTreeView
  // is gone: in this case we always expect |hasEventHandlers| to be false.
  DCHECK(!widget || widget->GetLayerTreeView() || !has_event_handlers);
  if (widget && widget->GetLayerTreeView())
    widget->GetLayerTreeView()->SetHaveScrollEventHandlers(has_event_handlers);
#endif
}

void ChromeClientImpl::SetNeedsLowLatencyInput(LocalFrame* frame,
                                               bool needs_low_latency) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRootFrameWidget();
  if (!widget)
    return;

  if (WebWidgetClient* client = widget->Client())
    client->SetNeedsLowLatencyInput(needs_low_latency);
#endif
}

void ChromeClientImpl::RequestUnbufferedInputEvents(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRootFrameWidget();
  if (!widget)
    return;

  if (WebWidgetClient* client = widget->Client())
    client->RequestUnbufferedInputEvents();
#endif
}

void ChromeClientImpl::SetTouchAction(LocalFrame* frame,
                                      TouchAction touch_action) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRootFrameWidget();
  if (!widget)
    return;

  if (WebWidgetClient* client = widget->Client())
    client->SetTouchAction(static_cast<TouchAction>(touch_action));
#endif
}

bool ChromeClientImpl::RequestPointerLock(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return WebLocalFrameImpl::FromFrame(frame)
      ->LocalRootFrameWidget()
      ->Client()
      ->RequestPointerLock();
#endif
}

void ChromeClientImpl::RequestPointerUnlock(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
#if 0
  return WebLocalFrameImpl::FromFrame(frame)
      ->LocalRootFrameWidget()
      ->Client()
      ->RequestPointerUnlock();
#endif
}

void ChromeClientImpl::DidAssociateFormControlsAfterLoad(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client = AutofillClientFromFrame(frame))
    fill_client->DidAssociateFormControlsDynamically();
#endif
}

void ChromeClientImpl::ShowVirtualKeyboardOnElementFocus(LocalFrame& frame) {
  ASSERT(false); // BKTODO:
#if 0
  WebLocalFrameImpl::FromFrame(frame)
      ->LocalRootFrameWidget()
      ->Client()
      ->ShowVirtualKeyboardOnElementFocus();
#endif
}

void ChromeClientImpl::OnMouseDown(Node& mouse_down_node) {
  if (auto* fill_client =
          AutofillClientFromFrame(mouse_down_node.GetDocument().GetFrame())) {
    ASSERT(false); // BKTODO:
#if 0
    fill_client->DidReceiveLeftMouseDownOrGestureTapInNode(
        WebNode(&mouse_down_node));
#endif
  }
}

void ChromeClientImpl::HandleKeyboardEventOnTextField(
    HTMLInputElement& input_element,
    KeyboardEvent& event) {
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client =
          AutofillClientFromFrame(input_element.GetDocument().GetFrame())) {
    fill_client->TextFieldDidReceiveKeyDown(WebInputElement(&input_element),
                                            WebKeyboardEventBuilder(event));
  }
#endif
}

void ChromeClientImpl::DidChangeValueInTextField(
    HTMLFormControlElement& element) {
  ASSERT(false); // BKTODO:
#if 0
  Document& doc = element.GetDocument();
  if (auto* fill_client = AutofillClientFromFrame(doc.GetFrame()))
    fill_client->TextFieldDidChange(WebFormControlElement(&element));

  // Value changes caused by |document.execCommand| calls should not be
  // interpreted as a user action. See https://crbug.com/764760.
  if (!doc.IsRunningExecCommand()) {
    UseCounter::Count(doc, doc.IsSecureContext()
                               ? WebFeature::kFieldEditInSecureContext
                               : WebFeature::kFieldEditInNonSecureContext);
    doc.MaybeQueueSendDidEditFieldInInsecureContext();
    web_view_->PageImportanceSignals()->SetHadFormInteraction();
  }
#endif
}

void ChromeClientImpl::DidEndEditingOnTextField(
    HTMLInputElement& input_element) {
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client =
          AutofillClientFromFrame(input_element.GetDocument().GetFrame())) {
    fill_client->TextFieldDidEndEditing(WebInputElement(&input_element));
  }
#endif
}

void ChromeClientImpl::OpenTextDataListChooser(HTMLInputElement& input) {
  NotifyPopupOpeningObservers();
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client =
          AutofillClientFromFrame(input.GetDocument().GetFrame())) {
    fill_client->OpenTextDataListChooser(WebInputElement(&input));
  }
#endif
}

void ChromeClientImpl::TextFieldDataListChanged(HTMLInputElement& input) {
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client =
          AutofillClientFromFrame(input.GetDocument().GetFrame())) {
    fill_client->DataListOptionsChanged(WebInputElement(&input));
  }
#endif
}

void ChromeClientImpl::DidChangeSelectionInSelectControl(
    HTMLFormControlElement& element) {
  ASSERT(false); // BKTODO:
#if 0
  Document& doc = element.GetDocument();
  if (auto* fill_client = AutofillClientFromFrame(doc.GetFrame()))
    fill_client->SelectControlDidChange(WebFormControlElement(&element));
#endif
}

void ChromeClientImpl::SelectFieldOptionsChanged(
    HTMLFormControlElement& element) {
  ASSERT(false); // BKTODO:
#if 0
  Document& doc = element.GetDocument();
  if (auto* fill_client = AutofillClientFromFrame(doc.GetFrame()))
    fill_client->SelectFieldOptionsChanged(WebFormControlElement(&element));
#endif
}

void ChromeClientImpl::AjaxSucceeded(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
#if 0
  if (auto* fill_client = AutofillClientFromFrame(frame))
    fill_client->AjaxSucceeded();
#endif
}

void ChromeClientImpl::RegisterViewportLayers() const {
  ASSERT(false); // BKTODO:
#if 0
  if (web_view_->RootGraphicsLayer() && web_view_->LayerTreeView())
    web_view_->RegisterViewportLayersWithCompositor();
#endif
}

void ChromeClientImpl::DidUpdateBrowserControls() const {
  ASSERT(false); // BKTODO: web_view_->DidUpdateBrowserControls();
}

void ChromeClientImpl::SetOverscrollBehavior(
    const cc::OverscrollBehavior& overscroll_behavior) {
  ASSERT(false); // BKTODO: web_view_->SetOverscrollBehavior(overscroll_behavior);
}

void ChromeClientImpl::RegisterPopupOpeningObserver(
    PopupOpeningObserver* observer) {
  DCHECK(observer);
  popup_opening_observers_.insert(observer);
}

void ChromeClientImpl::UnregisterPopupOpeningObserver(
    PopupOpeningObserver* observer) {
  DCHECK(popup_opening_observers_.Contains(observer));
  popup_opening_observers_.erase(observer);
}

void ChromeClientImpl::NotifyPopupOpeningObservers() const {
  const HeapHashSet<WeakMember<PopupOpeningObserver>> observers(
      popup_opening_observers_);
  ASSERT(false); // BKTODO:
#if 0
  for (const auto& observer : observers)
    observer->WillOpenPopup();
#endif
}

FloatSize ChromeClientImpl::ElasticOverscroll() const {
  ASSERT(false); // BKTODO:
  return FloatSize();
#if 0
  return web_view_->ElasticOverscroll();
#endif
}

WebAutofillClient* ChromeClientImpl::AutofillClientFromFrame(
    LocalFrame* frame) {
  if (!frame) {
    // It is possible to pass nullptr to this method. For instance the call from
    // OnMouseDown might be nullptr. See https://crbug.com/739199.
    return nullptr;
  }

  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return WebLocalFrameImpl::FromFrame(frame)->AutofillClient();
#endif
}

}  // namespace blink
