// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_store.h
// Description: ViewStore Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_VIEW_STORE_H
#define BLINKIT_BLINKIT_VIEW_STORE_H

#pragma once

#include "third_party/zed/include/zed/shared_mutex.hpp"

namespace BlinKit {

class WinWebView;

class ViewStore
{
public:
    WinWebView* Lookup(HWND hWnd) const;

    void OnNewView(HWND hWnd, WinWebView *newView);
    void OnViewDestroyed(HWND hWnd);
private:
    mutable zed::shared_mutex m_lock;
    std::unordered_map<HWND, WinWebView *> m_views;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_VIEW_STORE_H
