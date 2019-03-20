// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: main_frame.h
// Description: Main Window Class
//      Author: Ziming Li
//     Created: 2018-09-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TEST_MAIN_FRAME_H
#define BLINKIT_TEST_MAIN_FRAME_H

#pragma once

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS> FrameTraits;

class MainFrame final : public CWindowImpl<MainFrame, CWindow, FrameTraits>
                      , public BlinKit::BkViewImplT<MainFrame>
                      , public BlinKit::BkCrawlerClient
{
public:
    static MainFrame* CreateInstance(void);
private:
    MainFrame(void) = default;
    ~MainFrame(void);

    // BkViewClient
    void BKAPI DocumentReady(BkView *view);
    // BkCrawlerClient
    void BKAPI DocumentReady(BkCrawler *crawler);

    BlinKit::BkCrawler *m_crawler = nullptr;

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    void OnFinalMessage(HWND) override { delete this; }
public:
    BEGIN_MSG_MAP(MainFrame)
        CHAIN_MSG_MAP(BlinKit::BkViewImplT<MainFrame>)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
    END_MSG_MAP()
};

#endif // BLINKIT_TEST_MAIN_FRAME_H
