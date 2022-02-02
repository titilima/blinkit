// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_host.cpp
// Description: WebViewHost Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./web_view_host.h"

#include "blinkit/ui/web_view_impl.h"
#include "blinkit/ui/compositor/layer_tree_host.h"
#include "blinkit/ui/compositor/tasks/paint_ui_task.h"

namespace BlinKit {

WebViewHost::WebViewHost(const BkWebViewClient &client, PageVisibilityState visibilityState)
    : m_view(new WebViewImpl(client, visibilityState, DefaultBackgroundColor()))
    , m_layerTreeHost(std::make_unique<LayerTreeHost>())
{
}

WebViewHost::~WebViewHost(void)
{
    delete m_view;
}

void WebViewHost::EnterAnimationSession(void)
{
    double tick = monotonicallyIncreasingTime();
    m_view->BeginFrame(tick);
}

WebLayerTreeView* WebViewHost::GetLayerTreeView(void) const
{
    return m_layerTreeHost.get();
}

void WebViewHost::InitializeView(float scaleFactor)
{
    m_view->Initialize(this, scaleFactor);
    m_layerTreeHost->SetNeedsCommit();
}

void WebViewHost::LeaveAnimationSession(bool fullPaint)
{
    m_view->UpdateLifecycle();

    std::unique_ptr<PaintUITask> paintTask = PreparePaintTask();
    paintTask->SetBackgroundColor(m_view->BaseBackgroundColor());
    if (fullPaint)
        paintTask->SetFullPaint();

    m_layerTreeHost->Update(paintTask);
}

void WebViewHost::Resize(const IntSize &size)
{
    m_layerTreeHost->setViewportSize(size);
    m_view->Resize(size);
}

void WebViewHost::SetScaleFactor(float scaleFactor)
{
    m_view->SetScaleFactor(scaleFactor);
    m_layerTreeHost->SetNeedsCommit();
}

} // namespace BlinKit
