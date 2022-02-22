// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: win_main.cpp
// Description: Application Entry
//      Author: Ziming Li
//     Created: 2019-09-03
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include <Windows.h>
#include <tchar.h>
#include <bk_app.h>
#include <bk.hpp>

using namespace bk;

// #define TEST_CRAWLER_ONLY

static const char UserScript[] = R"(
({
})
)";

static const char ReadyCode[] = R"(
    var e = document.querySelector('h1');
    console.log(e.textContent);
)";

static const char URL[] = "https://example.org";

class Client final : public crawler_client_impl<Client>
{
public:
    int Run(const char *URL)
    {
        BkAppClient appClient = { 0 };
        appClient.SizeOfStruct = sizeof(BkAppClient);
        appClient.UserData = this;
        appClient.Exit = Exit;
        BkInitialize(&appClient);

        m_crawler = create_crawler();
        BkRunCrawler(m_crawler, URL);

        return BkRunMessageLoop();
    }
private:
    static void BKAPI Exit(void *pThis)
    {
        BkDestroyCrawler(reinterpret_cast<Client *>(pThis)->m_crawler);
    }

    bool get_config(int cfg, std::string &dst) override
    {
        switch (cfg)
        {
            case BK_CFG_OBJECT_SCRIPT:
                dst = UserScript;
                break;
            default:
                return false;
        }
        return true;
    }
    void document_ready(BkJSContext ctx) override
    {
        BkEvaluate(ctx, ReadyCode, std::size(ReadyCode) - 1, nullptr);
        ::PostQuitMessage(EXIT_SUCCESS);
    }

    BkCrawler m_crawler = nullptr;
};

#ifndef TEST_CRAWLER_ONLY
static void BKAPI DocumentReady(BkWebView v, void *)
{
    // BKTODO: Add test code here.
}

static bool CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result, void *)
{
    LRESULT r = 0;
    switch (Msg)
    {
        case WM_CREATE:
        {
            BkWebView v = BkGetWebView(hWnd);
            BkLoadUI(v, "res:ui.html");
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            break;
        default:
            return false;
    }
    *result = r;
    return true;
}

static const TCHAR ClassName[] = TEXT("BkTestWindow");

static bool InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = BkDefWindowProc;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = ClassName;
    return 0 != RegisterClass(&wc);
}

static int Run(HINSTANCE hInstance, int nShowCmd)
{
    if (!InitApplication(hInstance))
        return EXIT_FAILURE;

    BkWebViewClient client = { 0 };
    client.SizeOfStruct = sizeof(BkWebViewClient);
    client.DocumentReady = DocumentReady;
    client.ProcessMessage = WndProc;
    HWND hWnd = BkCreateWebViewWindow(ClassName, TEXT("BlinKit Test Program"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, nullptr, &client);
    if (nullptr == hWnd)
        return EXIT_FAILURE;

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    return BkRunMessageLoop();
}
#endif // TEST_CRAWLER_ONLY

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int nShowCmd)
{
#ifdef TEST_CRAWLER_ONLY
    Client client;
    return client.Run(URL);
#else
    BkInitialize(nullptr);
    return Run(hInstance, nShowCmd);
#endif
}
