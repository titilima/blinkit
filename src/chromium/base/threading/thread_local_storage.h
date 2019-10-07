// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: thread_local_storage.h
// Description: ThreadLocalStorage Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_THREAD_LOCAL_STORAGE_H
#define BLINKIT_BASE_THREAD_LOCAL_STORAGE_H

#pragma once

namespace base {

class ThreadLocalStorage
{
public:
    typedef void (*TLSDestructorFunc)(void *value);

    class Slot final
    {
    public:
        explicit Slot(TLSDestructorFunc destructor = nullptr);
        ~Slot(void);

        void* Get(void) const;
        void Set(void *value);
    };
};

} // namespace base

#endif // BLINKIT_BASE_THREAD_LOCAL_STORAGE_H
