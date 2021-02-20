// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: client_caller_store.cpp
// Description: ClientCallerStore Class
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "client_caller_store.h"

#include <mutex>
#include "blinkit/win/message_task.h"

namespace BlinKit {

#ifdef BLINKIT_CRAWLER_ONLY
#ifndef NDEBUG
bool SingletonClientCallerStore::IsClientThread(void) const
{
    return GetCurrentThreadId() == m_threadId;
}
#endif

void SingletonClientCallerStore::Post(const base::Location &, std::function<void()> &&task)
{
    MessageTask::Post(m_threadId, std::move(task));
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ClientCallerStoreImpl::ClientCallerImpl final : public ClientCaller
{
public:
    ClientCallerImpl(ATOM atom)
    {
        m_hMsgWnd = ::CreateWindow(MAKEINTATOM(atom), nullptr, 0,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            HWND_MESSAGE, nullptr, theModule, nullptr);
    }
    ~ClientCallerImpl(void)
    {
        ::PostMessage(m_hMsgWnd, WM_CLOSE, 0, 0);
    }
private:
    void Post(const base::Location &, std::function<void()> &&task) override
    {
        MessageTask::Post(m_hMsgWnd, std::move(task));
    }
#ifndef NDEBUG
    bool IsClientThread(void) const override { return ::GetCurrentThreadId() == GetWindowThreadProcessId(m_hMsgWnd, nullptr); }
#endif

    HWND m_hMsgWnd;
};

ClientCallerStoreImpl::ClientCallerStoreImpl(void)
{
    WNDCLASS wc      = { 0 };
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = theModule;
    wc.lpszClassName = TEXT("BkMessageWindow");
    m_atom           = ::RegisterClass(&wc);
}

ClientCallerStoreImpl::~ClientCallerStoreImpl(void)
{
    std::unique_lock lock(m_lock);
    for (auto &it : m_callerMap)
        delete it.second;
    m_callerMap.clear();
}

ClientCaller& ClientCallerStoreImpl::Acquire(void)
{
    DWORD tid = ::GetCurrentThreadId();

    std::unique_lock lock(m_lock);
    auto it = m_callerMap.find(tid);
    if (std::end(m_callerMap) != it)
        return *(it->second);

    ClientCallerImpl *ret = new ClientCallerImpl(m_atom);
    m_callerMap[tid] = ret;
    return *ret;
}

LRESULT CALLBACK ClientCallerStoreImpl::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (MessageTask::Process(uMsg, wParam, lParam))
        return 0;
    else
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

} // namespace BlinKit
