// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page_visibility_state.h
// Description: PageVisibilityState
//      Author: Ziming Li
//     Created: 2020-10-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PAGE_VISIBILITY_STATE_H
#define BLINKIT_BLINK_PAGE_VISIBILITY_STATE_H

#pragma once

namespace blink {

enum class PageVisibilityState {
    kVisible = 0,
    kHidden,
    kHiddenButPainting,

    kMinValue = kVisible,
    kMaxValue = kHiddenButPainting,
};

} // namespace blink

#endif // BLINKIT_BLINK_PAGE_VISIBILITY_STATE_H
