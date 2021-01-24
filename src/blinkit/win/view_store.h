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

namespace BlinKit {

class WinWebView;

class ViewStore
{
public:
    ViewStore(void) { ::InitializeSRWLock(&m_lock); }

    WinWebView* Lookup(HWND hWnd) const;

    void lock_shared(void) { ::AcquireSRWLockShared(&m_lock); }
    void unlock_shared(void) { ::ReleaseSRWLockShared(&m_lock); }
    void lock(void) { ::AcquireSRWLockExclusive(&m_lock); }
    void unlock(void) { ::ReleaseSRWLockExclusive(&m_lock); }

    void OnNewView(HWND hWnd, WinWebView *newView);
    void OnViewDestroyed(HWND hWnd);
private:
    SRWLOCK m_lock;
    std::unordered_map<HWND, WinWebView *> m_views;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_VIEW_STORE_H
