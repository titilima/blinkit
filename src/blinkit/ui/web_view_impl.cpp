// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_impl.cpp
// Description: WebViewImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "web_view_impl.h"

#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/page_scale_constraints_set.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/viewport_data.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/page/chrome_client_impl.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"

using namespace blink;
using namespace BlinKit;

WebViewImpl::WebViewImpl(PageVisibilityState visibilityState)
    : m_chromeClient(ChromeClientImpl::Create(this)), m_baseBackgroundColor(Color::kWhite)
{
    memset(&m_client, 0, sizeof(m_client));

    Page::PageClients pageClients;
    pageClients.chromeClient = m_chromeClient.get();
    pageClients.frameClient = this;
    m_page = Page::Create(pageClients);
    SetVisibilityState(visibilityState, true);

#if 0 // BKTODO:
    // TODO(dcheng): All WebViewImpls should have an associated LayerTreeView,
    // but for various reasons, that's not the case... WebView plugin, printing,
    // workers, and tests don't use a compositor in their WebViews. Sometimes
    // they avoid the compositor by using a null client, and sometimes by having
    // the client return a null compositor. We should make things more consistent
    // and clear.
    // For some reason this was not set when WidgetClient() is not provided,
    // even though there will be no LayerTreeView in that case either.
    if (WidgetClient() && WidgetClient()->AllowsBrokenNullLayerTreeView())
        page_->GetSettings().SetAcceleratedCompositingEnabled(false);

    AllInstances().insert(this);

    page_importance_signals_.SetObserver(client);
#endif
    m_resizeViewportAnchor = std::make_unique<ResizeViewportAnchor>(*m_page);
}

WebViewImpl::~WebViewImpl(void)
{
    m_page->WillBeDestroyed();
}

Color WebViewImpl::BaseBackgroundColor(void) const
{
    return m_baseBackgroundColor;
}

float WebViewImpl::ClampPageScaleFactorToLimits(float scaleFactor) const
{
    return GetPageScaleConstraintsSet().FinalConstraints().ClampToConstraints(scaleFactor);
}

IntSize WebViewImpl::ContentsSize(void) const {
    if (LayoutView *layoutView = GetPage()->GetFrame()->ContentLayoutObject())
        return layoutView->DocumentRect().Size();
    return IntSize();
}

void WebViewImpl::DidChangeContentsSize(void)
{
    LocalFrameView *view = GetPage()->GetFrame()->View();

    int verticalScrollbarWidth = 0;
    if (nullptr != view)
    {
        if (PaintLayerScrollableArea *layoutViewport = view->LayoutViewport())
        {
            Scrollbar *verticalScrollbar = layoutViewport->VerticalScrollbar();
            if (nullptr != verticalScrollbar && !verticalScrollbar->IsOverlayScrollbar())
                verticalScrollbarWidth = verticalScrollbar->Width();
        }
    }

    GetPageScaleConstraintsSet().DidChangeContentsSize(ContentsSize(), verticalScrollbarWidth, PageScaleFactor());
}

void WebViewImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    ASSERT(false); // BKTODO:
}

void WebViewImpl::DispatchDidFinishLoad(void)
{
    AutoGarbageCollector gc;
    const auto task = [this]
    {
        m_client.DocumentReady(m_client.UserData);
    };
    m_frame->GetTaskRunner(TaskType::kInternalLoading)->PostTask(FROM_HERE, task);
}

IntSize WebViewImpl::FrameSize(void)
{
    // The frame size should match the viewport size at minimum scale, since the
    // viewport must always be contained by the frame.
    FloatSize frameSize(m_size);
    frameSize.Scale(1 / MinimumPageScaleFactor());
    return ExpandedIntSize(frameSize);
}

PageScaleConstraintsSet& WebViewImpl::GetPageScaleConstraintsSet(void) const
{
    return m_page->GetPageScaleConstraintsSet();
}

void WebViewImpl::Initialize(void)
{
    m_frame = LocalFrame::Create(this, m_page.get());
    m_frame->Init();
}

bool WebViewImpl::IsAcceleratedCompositingActive(void) const
{
    return false; // BKTODO: Support GPU.
}

int WebViewImpl::LoadUI(const char *URI)
{
    GURL u(URI);
    if (u.SchemeIsHTTPOrHTTPS())
    {
        BKLOG("URLs are not supported: %s", URI);
        return BK_ERR_URI;
    }

    FrameLoadRequest request(nullptr, ResourceRequest(u));
    m_frame->Loader().StartNavigation(request);
    return BK_ERR_SUCCESS;
}

void WebViewImpl::MainFrameLayoutUpdated(void)
{
    // May be useful, leave it here.
}

IntSize WebViewImpl::MainFrameSize(void)
{
    // The frame size should match the viewport size at minimum scale, since the
    // viewport must always be contained by the frame.
    FloatSize frameSize(m_size);
    frameSize.Scale(1 / MinimumPageScaleFactor());
    return ExpandedIntSize(frameSize);
}

float WebViewImpl::MinimumPageScaleFactor(void) const
{
    return GetPageScaleConstraintsSet().FinalConstraints().minimum_scale;
}

float WebViewImpl::PageScaleFactor(void) const
{
    if (Page *page = GetPage())
        return page->GetVisualViewport().Scale();
    return 1.0;
}

void WebViewImpl::PaintContent(cc::PaintCanvas *canvas, const WebRect &rect)
{
    // This should only be used when compositing is not being used for this
    // WebView, and it is painting into the recording of its parent.
    ASSERT(!IsAcceleratedCompositingActive());
    PageWidgetDelegate::PaintContent(*m_page, canvas, rect, *m_page->MainFrame());
}

bool WebViewImpl::ProcessTitleChange(const std::string &title) const
{
    if (nullptr == m_client.TitleChange)
        return false;
    return m_client.TitleChange(title.c_str(), m_client.UserData);
}

void WebViewImpl::RefreshPageScaleFactor(void)
{
    Page *page = GetPage();
    if (nullptr == page)
        return;

    LocalFrame *mainFrame = page->MainFrame();
    if (nullptr == mainFrame)
        return;

    LocalFrameView *view = mainFrame->View();
    if (nullptr == view)
        return;

    UpdatePageDefinedViewportConstraints(mainFrame->GetDocument()->GetViewportData().GetViewportDescription());

    PageScaleConstraintsSet &pageScaleConstraints = GetPageScaleConstraintsSet();
    pageScaleConstraints.ComputeFinalConstraints();

    float newPageScaleFactor = PageScaleFactor();
    if (pageScaleConstraints.NeedsReset() && pageScaleConstraints.FinalConstraints().initial_scale != -1)
    {
        newPageScaleFactor = GetPageScaleConstraintsSet().FinalConstraints().initial_scale;
        pageScaleConstraints.SetNeedsReset(false);
    }
    SetPageScaleFactor(newPageScaleFactor);

    UpdateLayerTreeViewport();
}

void WebViewImpl::ResizeAfterLayout(void)
{
    Page *page = GetPage();
    ASSERT(nullptr != page->GetFrame());

    if (m_shouldAutoResize)
    {
        LocalFrameView *view = page->GetFrame()->View();
        WebSize frameSize = view->Size();
        if (frameSize != m_size)
        {
            m_size = frameSize;

            page->GetVisualViewport().SetSize(m_size);
            GetPageScaleConstraintsSet().DidChangeInitialContainingBlockSize(m_size);
            view->SetInitialViewportSize(m_size);

            ASSERT(false); // BKTODO:
#if 0
            client_->DidAutoResize(size_);
            SendResizeEventAndRepaint();
#endif
        }
    }

    if (GetPageScaleConstraintsSet().ConstraintsDirty())
        RefreshPageScaleFactor();

    m_resizeViewportAnchor->ResizeFrameView(MainFrameSize());
}

void WebViewImpl::ScheduleAnimation(void)
{
#if 0 // BKTODO:
    if (layer_tree_view_)
    {
        layer_tree_view_->SetNeedsBeginFrame();
        return;
    }
    if (client_)
        client_->WidgetClient()->ScheduleAnimation();
#endif
}

void WebViewImpl::SetClient(const BkWebViewClient &client)
{
    memset(&m_client, 0, sizeof(m_client));

    m_client.UserData = client.UserData;
    m_client.DocumentReady = client.DocumentReady;
    m_client.TitleChange = client.TitleChange;

    // Use `offsetof` macro for different client versions.
}

void WebViewImpl::SetPageScaleFactor(float scaleFactor)
{
    Page *page = GetPage();
    ASSERT(nullptr != page);

    scaleFactor = ClampPageScaleFactorToLimits(scaleFactor);
    if (scaleFactor == PageScaleFactor())
        return;

    page->GetVisualViewport().SetScale(scaleFactor);
}

void WebViewImpl::SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState)
{
    ASSERT(m_page);
    GetPage()->SetVisibilityState(visibilityState, isInitialState);

#if 0 // BKTODO:
    bool visible = visibilityState == PageVisibilityState::kVisible;
    if (layer_tree_view_ && !override_compositor_visibility_)
        layer_tree_view_->SetVisible(visible);
    GetPage()->GetPageScheduler()->SetPageVisible(visible);
#endif
}

void WebViewImpl::TransitionToCommittedForNewPage(void)
{
    // Check if we're shutting down.
    if (!m_page)
        return;

    IntSize initialSize = FrameSize();
    Color baseBackgroundColor = BaseBackgroundColor();

    LocalFrame *frame = m_page->GetFrame();
    frame->CreateView(initialSize, baseBackgroundColor);

    LocalFrameView *view = frame->View();
    view->SetInitialViewportSize(GetPageScaleConstraintsSet().InitialViewportSize());
    if (ShouldAutoResize())
    {
        ASSERT(false); // BKTODO:
#if 0
        view->EnableAutoSizeMode(MinAutoSize(), MaxAutoSize());
#endif
    }

#if 0 // BKTODO: Check if necessary
    view->SetInputEventsScaleForEmulation(input_events_scale_factor_for_emulation_);
    view->SetDisplayMode(DisplayMode());

    if (frame_widget_)
        frame_widget_->DidCreateLocalRootView();
#endif
}

void WebViewImpl::UpdateLayerTreeViewport(void)
{
    BKLOG("// BKTODO: LayerTreeViewport support.");
}

void WebViewImpl::UpdateMainFrameLayoutSize(void)
{
    if (m_shouldAutoResize)
        return;

    LocalFrame *frame = m_page->GetFrame();
    if (nullptr == frame)
        return;

    LocalFrameView *view = frame->View();
    if (nullptr == view)
        return;

    WebSize layoutSize = m_size;
    if (Settings::ViewportEnabled)
        layoutSize = GetPageScaleConstraintsSet().GetLayoutSize();
    if (Settings::ForceZeroLayoutHeight)
        layoutSize.height = 0;
    view->SetLayoutSize(layoutSize);
}

void WebViewImpl::UpdatePageDefinedViewportConstraints(const ViewportDescription &description)
{
    if (nullptr == GetPage() || (0 == m_size.width && 0 == m_size.height))
        return;

    // When viewport is disabled (non-mobile), we always use gpu rasterization.
    // Otherwise, on platforms that do support viewport tags, we only enable it
    // when they are present. But Why? Historically this was used to gate usage of
    // gpu rasterization to a smaller set of less complex cases to avoid driver
    // bugs dealing with websites designed for desktop. The concern is that on
    // older android devices (<L according to https://crbug.com/419521#c9),
    // drivers are more likely to encounter bugs with gpu raster when encountering
    // the full possibility of desktop web content. Further, Adreno devices <=L
    // have encountered problems that look like driver bugs when enabling
    // OOP-Raster which is gpu-based. Thus likely a blacklist would be required
    // for non-viewport-specified pages in order to avoid crashes or other
    // problems on mobile devices with gpu rasterization.
    bool viewportEnabled = Settings::ViewportEnabled;
#if 0 // BKTODO:
    matches_heuristics_for_gpu_rasterization_ =
        viewport_enabled ? description.MatchesHeuristicsForGpuRasterization()
        : true;
    if (layer_tree_view_) {
        layer_tree_view_->HeuristicsForGpuRasterizationUpdated(
            matches_heuristics_for_gpu_rasterization_);
    }
#endif

    if (!viewportEnabled)
    {
        GetPageScaleConstraintsSet().ClearPageDefinedConstraints();
        UpdateMainFrameLayoutSize();
        return;
    }

    ASSERT(false); // BKTODO:
#if 0
    Document* document = GetPage()->DeprecatedLocalMainFrame()->GetDocument();

    Length default_min_width =
        document->GetViewportData().ViewportDefaultMinWidth();
    if (default_min_width.IsAuto())
        default_min_width = Length(kExtendToZoom);

    ViewportDescription adjusted_description = description;
    if (SettingsImpl()->ViewportMetaLayoutSizeQuirk() &&
        adjusted_description.type == ViewportDescription::kViewportMeta) {
        const int kLegacyWidthSnappingMagicNumber = 320;
        if (adjusted_description.max_width.IsFixed() &&
            adjusted_description.max_width.Value() <=
            kLegacyWidthSnappingMagicNumber)
            adjusted_description.max_width = Length(kDeviceWidth);
        if (adjusted_description.max_height.IsFixed() &&
            adjusted_description.max_height.Value() <= size_.height)
            adjusted_description.max_height = Length(kDeviceHeight);
        adjusted_description.min_width = adjusted_description.max_width;
        adjusted_description.min_height = adjusted_description.max_height;
    }

    float old_initial_scale =
        GetPageScaleConstraintsSet().PageDefinedConstraints().initial_scale;
    GetPageScaleConstraintsSet().UpdatePageDefinedConstraints(
        adjusted_description, default_min_width);

    if (SettingsImpl()->ClobberUserAgentInitialScaleQuirk() &&
        GetPageScaleConstraintsSet().UserAgentConstraints().initial_scale != -1 &&
        GetPageScaleConstraintsSet().UserAgentConstraints().initial_scale *
        DeviceScaleFactor() <=
        1) {
        if (description.max_width == Length(kDeviceWidth) ||
            (description.max_width.GetType() == kAuto &&
                GetPageScaleConstraintsSet().PageDefinedConstraints().initial_scale ==
                1.0f))
            SetInitialPageScaleOverride(-1);
    }

    Settings& page_settings = GetPage()->GetSettings();
    GetPageScaleConstraintsSet().AdjustForAndroidWebViewQuirks(
        adjusted_description, default_min_width.IntValue(), DeviceScaleFactor(),
        SettingsImpl()->SupportDeprecatedTargetDensityDPI(),
        page_settings.GetWideViewportQuirkEnabled(),
        page_settings.GetUseWideViewport(),
        page_settings.GetLoadWithOverviewMode(),
        SettingsImpl()->ViewportMetaNonUserScalableQuirk());
    float new_initial_scale =
        GetPageScaleConstraintsSet().PageDefinedConstraints().initial_scale;
    if (old_initial_scale != new_initial_scale && new_initial_scale != -1) {
        GetPageScaleConstraintsSet().SetNeedsReset(true);
        if (MainFrameImpl() && MainFrameImpl()->GetFrameView())
            MainFrameImpl()->GetFrameView()->SetNeedsLayout();
    }

    if (TextAutosizer* text_autosizer = document->GetTextAutosizer())
        text_autosizer->UpdatePageInfoInAllFrames();

    UpdateMainFrameLayoutSize();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI)
{
    return view->LoadUI(URI);
}

BKEXPORT void BKAPI BkWebViewSetClient(BkWebView view, BkWebViewClient *client)
{
    view->SetClient(*client);
}

} // extern "C"
