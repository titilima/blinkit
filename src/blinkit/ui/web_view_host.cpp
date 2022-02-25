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

namespace BlinKit {

WebViewHost::WebViewHost(const BkWebViewClient &client, PageVisibilityState visibilityState)
    : m_client(client)
    , m_view(new WebViewImpl(*this, visibilityState, DefaultBackgroundColor()))
    , m_layerTreeHost(std::make_unique<LayerTreeHost>(*this))
{
}

WebViewHost::~WebViewHost(void)
{
    delete m_view;
}

void WebViewHost::Commit(void)
{
    m_layerTreeHost->Commit();
}

WebLayerTreeView* WebViewHost::GetLayerTreeView(void) const
{
    return m_layerTreeHost.get();
}

void WebViewHost::InitializeView(float scaleFactor)
{
    m_view->Initialize(scaleFactor);
    m_layerTreeHost->SetNeedsCommit();
}

void WebViewHost::ProcessDocumentReady(void)
{
    m_client.DocumentReady(m_view, m_client.UserData);
}

void WebViewHost::ProcessLoadURI(const char *URI)
{
    if (nullptr != m_client.LoadURI)
        m_client.LoadURI(m_view, URI, m_client.UserData);
}

void WebViewHost::ProcessSizeChanged(const IntSize &size)
{
    if (nullptr != m_client.SizeChanged)
        m_client.SizeChanged(m_view, size.width(), size.height(), m_client.UserData);
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
