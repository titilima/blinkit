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
    Client(void)
    {
        BkAppClient appClient = { 0 };
        appClient.SizeOfStruct = sizeof(BkAppClient);
        appClient.UserData = this;
        appClient.Exit = Exit;
        BkInitialize(BK_APP_MAINTHREAD_MODE, &appClient);
    }

    int Run(const char *URL)
    {
        m_crawler = create_crawler();
        BkRunCrawler(m_crawler, URL);
        return BkRunApp();   
    }
private:
    static void BKAPI Exit(void *pThis)
    {
        BkDestroyCrawler(reinterpret_cast<Client *>(pThis)->m_crawler);
    }

    bool get_crawler_config(int cfg, std::string &dst) const override
    {
        switch (cfg)
        {
            case BK_CFG_OBJECT_SCRIPT:
                dst.assign(UserScript);
                break;
            default:
                return crawler_client_impl::get_crawler_config(cfg, dst);
        }
        return true;
    }
    void document_ready(void) override
    {
        BkJSContext ctx = BkGetScriptContextFromCrawler(m_crawler);
        BkEvaluate(ctx, ReadyCode, nullptr);

        BkExitApp(EXIT_SUCCESS);
    }

    BkCrawler m_crawler = nullptr;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int nShowCmd)
{
    Client client;
    return client.Run(URL);
}
