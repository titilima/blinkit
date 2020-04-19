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

namespace BlinKit {

PosixApp::PosixApp(int mode) : AppImpl(mode)
{
}

PosixApp::~PosixApp(void) = default;

void PosixApp::Exit(int code)
{
    ASSERT(false); // BKTODO:
}

std::shared_ptr<base::SingleThreadTaskRunner> PosixApp::GetTaskRunner(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

int PosixApp::RunAndFinalize(void)
{
    ASSERT(false); // BKTODO:
    return 0;
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
    ASSERT(false); // BKTODO:
}

} // namespace BlinKit
