// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: simple_context.h
// Description: SimpleContext Class
//      Author: Ziming Li
//     Created: 2020-08-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SIMPLE_CONTEXT_H
#define BLINKIT_BLINKIT_SIMPLE_CONTEXT_H

#pragma once

#include "blinkit/js/context_impl.h"

namespace BlinKit {

class SimpleContext final : public ContextImpl
{
private:
    bool QueryDestroy(void) const override { return true; }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_SIMPLE_CONTEXT_H
