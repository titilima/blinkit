// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_app.cpp
// Description: AppleApp Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_app.h"

#include "base/memory/ptr_util.h"
#include "blinkit/app/app_caller_impl.h"
#include "blinkit/apple/cf.h"
#include "blinkit/apple/run_loop.h"

namespace BlinKit {

AppleApp::AppleApp(BkAppClient *client) : AppImpl(client)
{
}

AppleApp::~AppleApp(void) = default;

void AppleApp::Exit(int code)
{
    m_exitCode = code;
    CFRunLoopStop(CFRunLoopGetCurrent());
}

std::shared_ptr<base::SingleThreadTaskRunner> AppleApp::GetTaskRunner(void) const
{
    return m_runLoop->GetTaskRunner();
}

void AppleApp::Initialize(void)
{
    AppImpl::Initialize();
    m_runLoop = std::make_unique<RunLoop>();
}

bool AppleApp::InitializeForBackgroundMode(void)
{
    ASSERT(false); // BKTODO:
    return false;
}

int AppleApp::RunMessageLoop(void)
{
    int r = m_runLoop->Run();
    OnExit();
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef BLINKIT_CRAWLER_ONLY
std::unique_ptr<AppImpl> AppImpl::CreateInstanceForExclusiveMode(BkAppClient *client)
{
    AppleApp *app = new AppleApp(client);
    if (nullptr != app)
    {
        app->Initialize();
        app->m_appCaller = std::make_unique<SyncAppCallerImpl>();
        ASSERT(false); // BKTODO: app->m_clientCallerStore = std::make_unique<SingletonClientCallerStore>();
    }
    return base::WrapUnique(app);
}
#endif

bool AppImpl::InitializeForBackgroundMode(BkAppClient *client)
{
    AppleApp *app = new AppleApp(client);
    if (nullptr == app)
    {
        ASSERT(nullptr != app);
        return false;
    }

    return app->InitializeForBackgroundMode();
}

void AppImpl::Log(const char *s)
{
    CF::StaticString ss(s);
    CFShow(ss);
}

} // namespace BlinKit
