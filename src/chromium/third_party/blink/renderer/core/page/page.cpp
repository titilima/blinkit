// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page.cpp
// Description: Page Class
//      Author: Ziming Li
//     Created: 2020-10-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "page.h"

#include "third_party/blink/renderer/core/frame/local_frame.h"

namespace blink {

Page::Page(PageClients &pageClients)
    : m_chromeClient(pageClients.chromeClient)
    , m_frame(LocalFrame::Create(pageClients.frameClient))
{
}

Page::~Page(void) = default;

std::unique_ptr<Page> Page::Create(PageClients &pageClients)
{
    return base::WrapUnique(new Page(pageClients));
}

void Page::SetVisibilityState(PageVisibilityState visibilityState, bool isInitialState)
{
    if (m_visibilityState == visibilityState)
        return;
    m_visibilityState = visibilityState;

    if (isInitialState)
        return;
    ASSERT(false); // BKTODO:
#if 0
    NotifyPageVisibilityChanged();

    if (main_frame_) {
        if (IsPageVisible())
            RestoreSVGImageAnimations();
        main_frame_->DidChangeVisibilityState();
    }
#endif
}

} // namespace blink
