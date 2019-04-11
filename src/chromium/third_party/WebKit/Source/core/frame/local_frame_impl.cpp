// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame_impl.cpp
// Description: LocalFrameImpl Class
//      Author: Ziming Li
//     Created: 2019-04-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "local_frame_impl.h"

#include "core/frame/LocalDOMWindow.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/loader/NavigationScheduler.h"

#include "js/public/script_controller.h"

namespace blink {

LocalFrameImpl::LocalFrameImpl(FrameLoaderClient *client, FrameHost *host, FrameOwner *owner)
    : Frame(client, host, owner)
    , m_loader(this)
    , m_navigationScheduler(NavigationScheduler::create(this))
    , m_script(ScriptController::create(this))
{
}

Document* LocalFrameImpl::document(void) const
{
    return m_domWindow ? m_domWindow->document() : nullptr;
}

DOMWindow* LocalFrameImpl::domWindow(void) const
{
    return m_domWindow.get();
}

void LocalFrameImpl::navigate(Document &originDocument, const KURL &url, bool replaceCurrentItem, UserGestureStatus userGestureStatus)
{
    assert(false); // Not reached!
}

void LocalFrameImpl::navigate(const FrameLoadRequest & request)
{
    assert(false); // Not reached!
}

bool LocalFrameImpl::prepareForCommit(void)
{
    return loader().prepareForCommit();
}

void LocalFrameImpl::printNavigationErrorMessage(const Frame &targetFrame, const char *reason)
{
    assert(false); // Not reached!
}

void LocalFrameImpl::reload(FrameLoadType loadType, ClientRedirectPolicy clientRedirectPolicy)
{
    assert(false); // Not reached!
}

SecurityContext* LocalFrameImpl::securityContext(void) const
{
    return document();
}

bool LocalFrameImpl::shouldClose(void)
{
    return m_loader.shouldClose();
}

WindowProxy* LocalFrameImpl::windowProxy(DOMWrapperWorld &world)
{
    assert(false); // Not reached!
    return nullptr;
}

WindowProxyManager* LocalFrameImpl::windowProxyManager(void) const
{
    assert(false); // Not reached!
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FrameNavigationDisabler::FrameNavigationDisabler(LocalFrameImpl &frame) : m_frame(&frame)
{
    m_frame->DisableNavigation();
}

FrameNavigationDisabler::~FrameNavigationDisabler(void)
{
    m_frame->EnableNavigation();
}

} // namespace blink
