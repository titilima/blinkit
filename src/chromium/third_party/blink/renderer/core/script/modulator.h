// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: modulator.h
// Description: Modulator Classes
//      Author: Ziming Li
//     Created: 2020-12-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_MODULATOR_H
#define BLINKIT_BLINK_MODULATOR_H

#pragma once

#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

namespace blink {

class ModuleScript;

// A SingleModuleClient is notified when single module script node (node as in a
// module tree graph) load is complete and its corresponding entry is created in
// module map.
class SingleModuleClient : public GarbageCollectedFinalized<SingleModuleClient>
{
public:
    virtual ~SingleModuleClient(void) = default;
    virtual void Trace(Visitor *visitor) {}

    virtual void NotifyModuleLoadFinished(ModuleScript *) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_MODULATOR_H
