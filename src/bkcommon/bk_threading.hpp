// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_threading.hpp
// Description: Threading Helpers
//      Author: Ziming Li
//     Created: 2020-11-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BK_THREADING_HPP
#define BLINKIT_BKCOMMON_BK_THREADING_HPP

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>

namespace BlinKit {

template <typename T>
struct BkSignalTraits
{
    static bool Wait(const T &b)
    {
        NOTREACHED();
        exit(0);
    }
};

template <typename Data, typename Traits = BkSignalTraits<Data>>
class BkConditionImpl
{
public:
    using Accessor = std::function<void(Data &)>;

    void Wait(const Accessor &accessor = Accessor())
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!Traits::Wait(m_signalData))
            m_cond.wait(lock);
        accessor(m_signalData);
    }
protected:
    BkConditionImpl(const Data &initialData = Data()) : m_signalData(initialData) {}

    void Access(const Accessor &accessor)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        accessor(m_signalData);
    }
    void Notify(void)
    {
        m_cond.notify_one();
    }
private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    Data m_signalData;
};

template <> struct BkSignalTraits<bool> {
    static bool Wait(bool b) { return b; }
};

class BkSignal : public BkConditionImpl<bool>
{
public:
    BkSignal(void) : BkConditionImpl(false) {}

    void Signal(void)
    {
        Access([](bool &b) {
            b = true;
        });
        Notify();
    }
};

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_BK_THREADING_HPP
