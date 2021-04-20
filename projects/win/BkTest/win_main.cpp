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
        m_URL = URL;

        BkAppClient appClient = { 0 };
        appClient.SizeOfStruct = sizeof(BkAppClient);
        appClient.UserData = this;
        appClient.Exit = Exit;
        return BkCrawlerMain(&appClient, Init);
    }
private:
    static void BKAPI Init(void *pThis)
    {
        reinterpret_cast<Client *>(pThis)->init();
    }
    static void BKAPI Exit(void *pThis)
    {
        BkDestroyCrawler(reinterpret_cast<Client *>(pThis)->m_crawler);
    }

    void init(void)
    {
        m_crawler = create_crawler();
        BkRunCrawler(m_crawler, URL);
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
    void document_ready(void) override
    {
        auto callback = [](BkJSContext ctx)
        {
            BkEvaluate(ctx, ReadyCode, std::size(ReadyCode) - 1, nullptr);
            BkExit(EXIT_SUCCESS);
        };
        js_call::prepare(callback)->commit_to(m_crawler);
    }

    BkCrawler m_crawler = nullptr;
    std::string m_URL;
};

#ifndef TEST_CRAWLER_ONLY
static void BKAPI DocumentReady(void *)
{
    // BKTODO: Add test code here.
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT r = 0;
    switch (Msg)
    {
        case WM_CREATE:
        {
            BkWebView v = BkGetWebView(hWnd);

            BkWebViewClient client = { 0 };
            client.SizeOfStruct = sizeof(BkWebViewClient);
            client.DocumentReady = DocumentReady;
            BkWebViewSetClient(v, &client);

            BkLoadUI(v, "res:ui.html");
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            break;
        default:
            r = BkDefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return r;
}

static const TCHAR ClassName[] = TEXT("BkTestWindow");

static bool InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = ClassName;
    return 0 != RegisterClass(&wc);
}

static int Run(HINSTANCE hInstance, int nShowCmd)
{
    if (!InitApplication(hInstance))
        return EXIT_FAILURE;

    HWND hWnd = CreateWindow(ClassName, TEXT("BlinKit Test Program"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, nullptr, nullptr);
    if (nullptr == hWnd)
        return EXIT_FAILURE;

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return EXIT_SUCCESS;
}
#endif // TEST_CRAWLER_ONLY

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int nShowCmd)
{
#ifdef TEST_CRAWLER_ONLY
    Client client;
    return client.Run(URL);
#else
    int r;

    BkInitialize(nullptr);
    r = Run(hInstance, nShowCmd);
    BkFinalize();
    return r;
#endif
}
