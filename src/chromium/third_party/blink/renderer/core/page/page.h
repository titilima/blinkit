// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page.h
// Description: Page Class
//      Author: Ziming Li
//     Created: 2020-09-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PAGE_H
#define BLINKIT_BLINK_PAGE_H

#pragma once

#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"

namespace blink {

class LocalFrame;
class LocalFrameClient;
class PageScaleConstraintsSet;

class Page
{
public:
    ~Page(void);

    // It is up to the platform to ensure that non-null clients are provided where
    // required.
    struct PageClients final {
        STACK_ALLOCATED();
    public:
        PageClients(void) = default;

        ChromeClient *chromeClient = nullptr;
        LocalFrameClient *frameClient = nullptr;
        DISALLOW_COPY_AND_ASSIGN(PageClients);
    };

    static std::unique_ptr<Page> Create(PageClients &pageClients);

    LocalFrame* GetFrame(void) const { return m_frame.get(); }
    PageScaleConstraintsSet& GetPageScaleConstraintsSet(void);
    const PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;
    PageVisibilityState VisibilityState(void) const { return m_visibilityState; }
    void SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState);
private:
    explicit Page(PageClients &pageClients);

    ChromeClient *m_chromeClient;
    std::unique_ptr<LocalFrame> m_frame;
    std::unique_ptr<PageScaleConstraintsSet> m_pageScaleConstraintsSet;
    PageVisibilityState m_visibilityState = PageVisibilityState::kVisible;
};

} // namespace blink

#endif // BLINKIT_BLINK_PAGE_H
