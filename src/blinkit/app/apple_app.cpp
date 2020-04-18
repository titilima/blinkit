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

#include "bk_app.h"
#include "blinkit/apple/cf.h"
#include "blinkit/blink_impl/apple_task_runner.h"

namespace BlinKit {

static std::shared_ptr<base::SingleThreadTaskRunner> CreateTaskRunner(int appMode)
{
    switch (appMode)
    {
        case BK_APP_MAINTHREAD_MODE:
            return std::make_shared<MainThreadTaskRunner>();
        default:
            NOTREACHED();
    }
    return nullptr;
}

AppleApp::AppleApp(int mode) : AppImpl(mode), m_taskRunner(CreateTaskRunner(mode))
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
    return m_taskRunner;
}

int AppleApp::RunAndFinalize(void)
{
    CFRunLoopRun();
    return m_exitCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AppImpl* AppImpl::CreateInstance(int mode)
{
    return new AppleApp(mode);
}

void AppImpl::InitializeBackgroundInstance(void)
{
    ASSERT(false); // BKTODO:
}

void AppImpl::Log(const char *s)
{
    CF::StaticString ss(s);
    CFShow(ss);
}

} // namespace BlinKit
