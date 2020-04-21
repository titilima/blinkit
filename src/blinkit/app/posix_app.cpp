// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_app.cpp
// Description: PosixApp Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_app.h"

#include "bk_app.h"
#include "blinkit/blink_impl/posix_task_runner.h"
#include "blinkit/posix/task_loop.h"

namespace BlinKit {

PosixApp::PosixApp(int mode) : AppImpl(mode), m_taskLoop(std::make_unique<TaskLoop>())
{
}

PosixApp::~PosixApp(void) = default;

void PosixApp::Exit(int code)
{
    m_taskLoop->Exit(code);
}

std::shared_ptr<base::SingleThreadTaskRunner> PosixApp::GetTaskRunner(void) const
{
    return m_taskLoop->GetTaskRunner();
}

int PosixApp::RunAndFinalize(void)
{
    int exitCode = m_taskLoop->Run();
    delete this;
    return exitCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AppImpl* AppImpl::CreateInstance(int mode)
{
    return new PosixApp(mode);
}

void AppImpl::InitializeBackgroundInstance(void)
{
    ASSERT(false); // BKTODO:
}

void AppImpl::Log(const char *s)
{
    fputs(s, stderr);
}

} // namespace BlinKit
