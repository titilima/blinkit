// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: InitModules.cpp
// Description: ModulesInitializer Class
//      Author: Ziming Li
//     Created: 2018-08-15
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "InitModules.h"

namespace blink {

void ModulesInitializer::init(void)
{
    ASSERT(!isInitialized());

    CoreInitializer::init();

    ASSERT(isInitialized());
}

void ModulesInitializer::terminateThreads(void)
{
    // Nothing to do.
}

} // namespace blink
