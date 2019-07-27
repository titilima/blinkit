// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: main_frame.cpp
// Description: Main Window Class
//      Author: Ziming Li
//     Created: 2018-09-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "main_frame.h"

MainFrame::~MainFrame(void)
{
    if (nullptr != m_crawler)
        m_crawler->Destroy();
}

MainFrame* MainFrame::CreateInstance(void)
{
    MainFrame *w = new MainFrame;
    assert(nullptr != w);

    if (nullptr != w)
    {
        HWND h = w->Create(nullptr);
        if (nullptr != h)
            return w;

        assert(nullptr != h);
        delete w;
    }

    return nullptr;
}

void BKAPI MainFrame::DocumentReady(BkView *view)
{
#if 0 // TODO: Enable crawler if necessary.
    m_crawler = BkGetApp()->CreateCrawler(*this);
    m_crawler->Load("http://127.0.0.1:8000/target.html");
#endif
}

void BKAPI MainFrame::DocumentReady(BkCrawler *crawler)
{
    assert(false); // TODO:
}

void BKAPI MainFrame::GetUserScript(BkBuffer &userScript)
{
    assert(false); // TODO:
}

LRESULT MainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    GetView()->Load("res:ui.html");
    return 0;
}

LRESULT MainFrame::OnDestroy(UINT, WPARAM, LPARAM, BOOL &)
{
    PostQuitMessage(EXIT_SUCCESS);
    return 0;
}
