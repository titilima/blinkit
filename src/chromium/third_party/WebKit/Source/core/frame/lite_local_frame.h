// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: lite_local_frame.h
// Description: LiteLocalFrame Class
//      Author: Ziming Li
//     Created: 2019-04-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LITE_LOCAL_FRAME_H
#define BLINKIT_BLINK_LITE_LOCAL_FRAME_H

#pragma once

#include "core/frame/Frame.h"
#include "core/loader/FrameLoader.h"

namespace blink {

class LocalDOMWindow;
class NavigationScheduler;
class ScriptController;

class LiteLocalFrame : public Frame
{
public:
    static PassRefPtrWillBeRawPtr<LiteLocalFrame> create(FrameLoaderClient *client);

    void init(void) { m_loader.init(); }

    Document* document(void) const;
    FrameLoader& loader(void) const { return m_loader; }
    LocalDOMWindow* localDOMWindow(void) const { return m_domWindow.get(); }
    NavigationScheduler& navigationScheduler(void) const {
        ASSERT(m_navigationScheduler);
        return *m_navigationScheduler.get();
    }
    ScriptController& script(void) const { return *m_script; }

    DOMWindow* domWindow(void) const override final;
    WindowProxy* windowProxy(DOMWrapperWorld &) override final;
    void navigate(Document &originDocument, const KURL &, bool replaceCurrentItem, UserGestureStatus) override;
    void navigate(const FrameLoadRequest &) override;
    void reload(FrameLoadType, ClientRedirectPolicy) override;
    bool shouldClose(void) override final;
    SecurityContext* securityContext(void) const override final;
    void printNavigationErrorMessage(const Frame &, const char *reason) override;
    bool prepareForCommit(void) override final;

    bool isNavigationAllowed(void) const { return 0 == m_navigationDisableCount; }
protected:
    LiteLocalFrame(FrameLoaderClient *, FrameHost *, FrameOwner *);

    mutable FrameLoader m_loader;
    OwnPtrWillBeMember<NavigationScheduler> m_navigationScheduler;
    RefPtrWillBeMember<LocalDOMWindow> m_domWindow;
    const OwnPtrWillBeMember<ScriptController> m_script;
private:
    friend class FrameNavigationDisabler;

    void EnableNavigation(void) { --m_navigationDisableCount; }
    void DisableNavigation(void) { ++m_navigationDisableCount; }

    Type GetType(void) const override { return Type::LiteLocal; }
    WindowProxyManager* windowProxyManager(void) const override final;

    int m_navigationDisableCount = 0;
};

class FrameNavigationDisabler {
    WTF_MAKE_NONCOPYABLE(FrameNavigationDisabler);
    STACK_ALLOCATED();
public:
    explicit FrameNavigationDisabler(LiteLocalFrame &);
    ~FrameNavigationDisabler(void);
private:
    RawPtrWillBeMember<LiteLocalFrame> m_frame;
};

} // namespace blink

#endif // BLINKIT_BLINK_LITE_LOCAL_FRAME_H
