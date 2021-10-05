#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: garbage_collector.h
// Description: GarbageCollector Class
//      Author: Ziming Li
//     Created: 2021-09-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GARBAGE_COLLECTOR_H
#define BLINKIT_BLINKIT_GARBAGE_COLLECTOR_H

#include <functional>

namespace blink {
class Visitor;
}

namespace BlinKit {

class GCObject;
struct GCSession;

class GarbageCollector final
{
public:
    template <class T>
    static void PerformOnRoot(T &r)
    {
        GarbageCollector gc([&r](blink::Visitor *visitor) { r.trace(visitor); });
        gc.Perform();
    }

    static void PerformOnMember(GCObject &o);
private:
    using Tracer = std::function<void(blink::Visitor *)>;

    GarbageCollector(GCObject &o);
    GarbageCollector(const Tracer &tracer);
    ~GarbageCollector(void);

    void Initialize(void);
    void Perform(void);

    Tracer m_tracer;
    std::unique_ptr<GCSession> m_session;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GARBAGE_COLLECTOR_H
