// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: supplementable.h
// Description: Supplementable Classes
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_SUPPLEMENTABLE_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_SUPPLEMENTABLE_H_

#include <unordered_map>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

#if DCHECK_IS_ON()
#include "third_party/blink/renderer/platform/wtf/threading.h"
#endif

namespace blink {

// What you should know about Supplementable and Supplement
// ========================================================
// Supplementable allows a garbage-collected object to be extended with
// additional data.
//
// Most commonly, this is used to attach data to a central object, such as
// LocalFrame, so that it can be easily accessed. This is similar to adding a
// member to that class (e.g. it is kept alive while the supplementable is),
// except that it occupies less memory if not used, and can be done in cases
// that would otherwise be a layering violation. For example, it is common for
// features implemented in modules/ to supplement classes in core/.
//
// Supplementable and Supplement instances are meant to be thread local. They
// should only be accessed from within the thread that created them. The
// 2 classes are not designed for safe access from another thread. Violating
// this design assumption can result in memory corruption and unpredictable
// behavior.
//
// What you should know about the Supplement keys
// ==============================================
// The Supplement is expected to use the same const char* string instance
// as its key. The Supplementable's SupplementMap will use the address of the
// string as the key and not the characters themselves. Hence, 2 strings with
// the same characters will be treated as 2 different keys.
//
// In practice, this is mostly hidden. Each Supplement must expose a static
// const char array which provides a human-readable key. Access to supplements
// requires passing the Supplement type, so these cannot collide for unequal
// types.
//
// Use extreme caution when deriving a supplementable class, as misuse can cause
// type confusion.
//
// Typical use is expected to look like this:
//
//     class NavigatorFoo : public Supplement<Navigator> {
//      public:
//       static const char kSupplementName[];
//
//       NavigatorFoo& From(Navigator&);
//     }
//
//     // static
//     const char NavigatorFoo::kSupplementName[] = "NavigatorFoo";
//
//     NavigatorFoo& NavigatorFoo::From(Navigator& navigator)
//     {
//       NavigatorFoo* supplement =
//           Supplement<Navigator>::From<NavigatorFoo>(navigator);
//       if (!supplement) {
//         supplement = new NavigatorFoo(navigator);
//         ProvideTo(navigator, supplement);
//       }
//       return *supplement;
//     }
//
// The hash map key will automatically be determined from the supplement type
// used.
//
// What you should know about thread checks
// ========================================
// When assertion is enabled this class performs thread-safety check so that
// supplements are provided to and from the same thread.
// If you want to provide some value for Workers, this thread check may be too
// strict, since in you'll be providing the value while worker preparation is
// being done on the main thread, even before the worker thread has started.
// If that's the case you can explicitly call reattachThread() when the
// Supplementable object is passed to the final destination thread (i.e.
// worker thread). This will allow supplements to be accessed on that thread.
// Please be extremely careful to use the method though, as randomly calling
// the method could easily cause racy condition.
//
// Note that reattachThread() does nothing if assertion is not enabled.

template <typename T>
class Supplementable;

template <typename T>
class Supplement : public GarbageCollectedMixin {
 public:
  // TODO(haraken): Remove the default constructor.
  // All Supplement objects should be instantiated with |supplementable_|.
  Supplement() {}

  explicit Supplement(T& supplementable) : supplementable_(&supplementable) {}

  virtual ~Supplement(void) = default;

  virtual void* GCObject(void) = 0;

  // Supplementable and its supplements live and die together.
  // Thus supplementable() should never return null (if the default constructor
  // is completely removed).
  T* GetSupplementable() const { return supplementable_; }

  template <typename SupplementType>
  static void ProvideTo(Supplementable<T>& supplementable,
                        SupplementType* supplement) {
    supplementable.ProvideSupplement(supplement);
  }

  template <typename SupplementType>
  static SupplementType* From(const Supplementable<T>& supplementable) {
    return supplementable.template RequireSupplement<SupplementType>();
  }

  template <typename SupplementType>
  static SupplementType* From(const Supplementable<T>* supplementable) {
    return supplementable
               ? supplementable->template RequireSupplement<SupplementType>()
               : nullptr;
  }

  void Trace(blink::Visitor* visitor) override {
    visitor->Trace(supplementable_);
  }

 private:
  Member<T> supplementable_;
};

template <typename T>
class Supplementable : public GarbageCollectedMixin {
  WTF_MAKE_NONCOPYABLE(Supplementable);

 public:
  template <typename SupplementType>
  void ProvideSupplement(SupplementType* supplement) {
#if DCHECK_IS_ON()
    DCHECK_EQ(creation_thread_id_, CurrentThread());
#endif
    static_assert(
        std::is_array<decltype(SupplementType::kSupplementName)>::value,
        "Declare a const char array kSupplementName. See Supplementable.h for "
        "details.");
    this->supplements_.insert({ SupplementType::kSupplementName, supplement });
  }

  template <typename SupplementType>
  void RemoveSupplement() {
#if DCHECK_IS_ON()
    DCHECK_EQ(creation_thread_id_, CurrentThread());
#endif
    static_assert(
        std::is_array<decltype(SupplementType::kSupplementName)>::value,
        "Declare a const char array kSupplementName. See Supplementable.h for "
        "details.");
    this->supplements_.erase(SupplementType::kSupplementName);
  }

  template <typename SupplementType>
  SupplementType* RequireSupplement() const {
#if DCHECK_IS_ON()
    DCHECK_EQ(attached_thread_id_, CurrentThread());
#endif
    static_assert(
        std::is_array<decltype(SupplementType::kSupplementName)>::value,
        "Declare a const char array kSupplementName. See Supplementable.h for "
        "details.");
    auto it = this->supplements_.find(SupplementType::kSupplementName);
    if (std::end(this->supplements_) == it)
      return nullptr;
    return static_cast<SupplementType *>(it->second.Get());
  }

  void ReattachThread() {
#if DCHECK_IS_ON()
    attached_thread_id_ = CurrentThread();
#endif
  }

  void Trace(blink::Visitor* visitor) override
  {
    for (auto it : supplements_)
      visitor->Trace(it.second->GCObject());
  }

 protected:
  using SupplementMap = std::unordered_map<const char *, Member<Supplement<T>>>;
  SupplementMap supplements_;

  Supplementable()
#if DCHECK_IS_ON()
      : attached_thread_id_(CurrentThread()),
        creation_thread_id_(CurrentThread())
#endif
  {
  }

#if DCHECK_IS_ON()
 private:
  ThreadIdentifier attached_thread_id_;
  ThreadIdentifier creation_thread_id_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_SUPPLEMENTABLE_H_
