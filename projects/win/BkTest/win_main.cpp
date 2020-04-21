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
#include <bk_js.h>
#include <BlinKit.hpp>

using namespace BlinKit;

static const char UserScript[] = R"(
({
})
)";

static const char ReadyCode[] = R"(
    var e = document.querySelector('h1');
    console.log(e.textContent);
)";

static const char URL[] = "https://example.org";

class Client final : public BkCrawlerClientImpl
{
public:
    Client(void)
    {
        memset(&m_appClient, 0, sizeof(BkAppClient));
        m_appClient.SizeOfStruct = sizeof(BkAppClient);
        m_appClient.UserData = this;
        m_appClient.Exit = Exit;
    }

    BkAppClient* GetAppClient(void) { return &m_appClient; }
    int Run(const char *URL)
    {
        m_crawler = BkCreateCrawler(*this);
        BkRunCrawler(m_crawler, URL);
        return BkRunApp();   
    }
private:
    static void BKAPI Exit(void *pThis)
    {
        BkDestroyCrawler(reinterpret_cast<Client *>(pThis)->m_crawler);
    }

    std::string GetCrawlerConfig(int cfg) override
    {
        switch (cfg)
        {
            case BK_CFG_OBJECT_SCRIPT:
                return UserScript;
        }
        return std::string();
    }
    void DocumentReady(void) override
    {
        BkJSContext ctx = BkGetScriptContextFromCrawler(m_crawler);
        BkJSEvaluate(ctx, ReadyCode, BK_EVAL_IGNORE_RETURN_VALUE);

        BkExitApp(EXIT_SUCCESS);
    }

    BkAppClient m_appClient;
    BkCrawler m_crawler = nullptr;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int nShowCmd)
{
    Client client;
    BkInitialize(BK_APP_MAINTHREAD_MODE, client.GetAppClient());
    return client.Run(URL);
}
