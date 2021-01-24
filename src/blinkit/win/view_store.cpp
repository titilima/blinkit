// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_store.cpp
// Description: ViewStore Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "view_store.h"

#include <mutex>
#include <shared_mutex>

namespace BlinKit {

WinWebView* ViewStore::Lookup(HWND hWnd) const
{
    std::shared_lock<ViewStore> lock(const_cast<ViewStore &>(*this));
    auto it = m_views.find(hWnd);
    if (std::end(m_views) != it)
        return it->second;
    return nullptr;;
}

void ViewStore::OnNewView(HWND hWnd, WinWebView *newView)
{
    std::unique_lock<ViewStore> lock(*this);
    ASSERT(std::end(m_views) == m_views.find(hWnd));
    m_views[hWnd] = newView;
}

void ViewStore::OnViewDestroyed(HWND hWnd)
{
    std::unique_lock<ViewStore> lock(*this);
    ASSERT(std::end(m_views) != m_views.find(hWnd));
    m_views.erase(hWnd);
}

} // namespace BlinKit
