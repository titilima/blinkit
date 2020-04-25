// -------------------------------------------------
// BlinKit SDK: Crawler Example
// -------------------------------------------------
//   File Name: main.cpp
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include <bk_app.h>
#include <bk_js.h>
#include <BlinKit.hpp>

using namespace BlinKit;

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

    void DocumentReady(void) override
    {
        BkJSContext ctx = BkGetScriptContextFromCrawler(m_crawler);

        static const char ReadyCode[] = R"(
            var e = document.querySelector('h1');
            console.log(e.textContent);
        )";
        BkJSEvaluate(ctx, ReadyCode, BK_EVAL_IGNORE_RETURN_VALUE);

        BkExitApp(EXIT_SUCCESS);
    }

    BkAppClient m_appClient;
    BkCrawler m_crawler = nullptr;
};

int main(void)
{
    Client client;
    BkInitialize(BK_APP_MAINTHREAD_MODE, client.GetAppClient());
    return client.Run("https://example.org");
}
