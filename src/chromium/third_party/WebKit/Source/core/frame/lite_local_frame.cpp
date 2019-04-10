// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: lite_local_frame.cpp
// Description: LiteLocalFrame Class
//      Author: Ziming Li
//     Created: 2019-04-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "lite_local_frame.h"

#include "core/frame/LocalDOMWindow.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/loader/NavigationScheduler.h"

#include "js/public/script_controller.h"

namespace blink {

LiteLocalFrame::LiteLocalFrame(FrameLoaderClient *client, FrameHost *host, FrameOwner *owner)
    : Frame(client, host, owner)
    , m_loader(this)
    , m_navigationScheduler(NavigationScheduler::create(this))
    , m_script(ScriptController::create(this))
{
}

PassRefPtrWillBeRawPtr<LiteLocalFrame> LiteLocalFrame::create(FrameLoaderClient *client)
{
    RefPtrWillBeRawPtr<LiteLocalFrame> frame = adoptRefWillBeNoop(new LiteLocalFrame(client, nullptr, nullptr));
    return frame.release();
}

Document* LiteLocalFrame::document(void) const
{
    return m_domWindow ? m_domWindow->document() : nullptr;
}

DOMWindow* LiteLocalFrame::domWindow(void) const
{
    return m_domWindow.get();
}

void LiteLocalFrame::navigate(Document &originDocument, const KURL &url, bool replaceCurrentItem, UserGestureStatus userGestureStatus)
{
    assert(false); // Not reached!
}

void LiteLocalFrame::navigate(const FrameLoadRequest & request)
{
    assert(false); // Not reached!
}

bool LiteLocalFrame::prepareForCommit(void)
{
    return loader().prepareForCommit();
}

void LiteLocalFrame::printNavigationErrorMessage(const Frame &targetFrame, const char *reason)
{
    assert(false); // Not reached!
}

void LiteLocalFrame::reload(FrameLoadType loadType, ClientRedirectPolicy clientRedirectPolicy)
{
    assert(false); // Not reached!
}

SecurityContext* LiteLocalFrame::securityContext(void) const
{
    return document();
}

bool LiteLocalFrame::shouldClose(void)
{
    return m_loader.shouldClose();
}

WindowProxy* LiteLocalFrame::windowProxy(DOMWrapperWorld &world)
{
    assert(false); // Not reached!
    return nullptr;
}

WindowProxyManager* LiteLocalFrame::windowProxyManager(void) const
{
    assert(false); // Not reached!
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FrameNavigationDisabler::FrameNavigationDisabler(LiteLocalFrame &frame) : m_frame(&frame)
{
    m_frame->DisableNavigation();
}

FrameNavigationDisabler::~FrameNavigationDisabler(void)
{
    m_frame->EnableNavigation();
}

} // namespace blink
