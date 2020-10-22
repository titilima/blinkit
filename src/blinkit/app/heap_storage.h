// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: heap_storage.h
// Description: HeapStorage Class
//      Author: Ziming Li
//     Created: 2020-10-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HEAP_STORAGE_H
#define BLINKIT_BLINKIT_HEAP_STORAGE_H

#pragma once

#include <functional>
#include <unordered_map>

namespace BlinKit {

class HeapStorage
{
public:
    ~HeapStorage(void);

    template <typename T>
    static void SavePersistentObject(T &obj)
    {
        T *p = &obj;
        if (theStorage->PersistentObjectSaved(p))
            return;

        theStorage->SavePersistentObjectImpl(p, std::bind(std::default_delete<T>(), p));
    }
    template <typename T>
    static void SavePersistentObject(const T &obj)
    {
        SavePersistentObject(const_cast<T &>(obj));
    }
private:
    friend class AppImpl;
    HeapStorage(void);

    bool PersistentObjectSaved(void *p);

    using Finalizer = std::function<void()>;
    void SavePersistentObjectImpl(void *p, Finalizer &&finalizer);

    static HeapStorage *theStorage;
    std::unordered_map<void *, std::function<void()>> m_persistentObjectFinalizers;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HEAP_STORAGE_H
