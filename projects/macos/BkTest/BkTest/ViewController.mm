// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: ViewController.mm
// Description: ViewController class
//      Author: Ziming Li
//     Created: 2019-08-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#import "ViewController.h"
#import <BkCrawler/BlinKit.h>

using namespace BlinKit;

static const char code[] = R"(
({
    scriptEnabled: false,
    documentReady: function() {
        // TODO: Add crawler code here.
    }
});
)";

class CrawlerController final : public BkCrawlerClient
{
public:
    CrawlerController(__weak ViewController *controller)
        : m_controller(controller), m_crawler(BkGetApp()->CreateCrawler(*this))
    {
        // Nothing
    }
    ~CrawlerController(void)
    {
        m_crawler->Destroy();
    }
    void Load(const char *URL)
    {
        m_crawler->Load(URL);
    }
private:
    void BKAPI GetUserScript(BkBuffer &userScript) override
    {
        userScript.Assign(code);
    }
    void BKAPI RequestComplete(BkCrawler *crawler, BkCrawlerResponse *response) override
    {
        response->Continue();
    }
    void BKAPI DocumentReady(BkCrawler *crawler) override
    {
        crawler->CallCrawler("documentReady");
    }

    __weak ViewController *m_controller;
    BkCrawler *m_crawler;
};

@interface ViewController()
{
@private
    std::unique_ptr<CrawlerController> m_crawlerController;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    m_crawlerController = std::make_unique<CrawlerController>(self);
    // TODO: m_crawlerController->Load("https://example.org");
}

- (void)setRepresentedObject: (id)representedObject
{
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
