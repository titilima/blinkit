// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: assertions.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003, 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_ASSERTIONS_H
#define BLINKIT_BLINK_ASSERTIONS_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/wtf_export.h"

#define SECURITY_CHECK  ASSERT
#define SECURITY_DCHECK ASSERT

// DEFINE_COMPARISON_OPERATORS_WITH_REFERENCES
// Allow equality comparisons of Objects by reference or pointer,
// interchangeably.  This can be only used on types whose equality makes no
// other sense than pointer equality.
#define DEFINE_COMPARISON_OPERATORS_WITH_REFERENCES(Type)                    \
  inline bool operator==(const Type& a, const Type& b) { return &a == &b; }  \
  inline bool operator==(const Type& a, const Type* b) { return &a == b; }   \
  inline bool operator==(const Type* a, const Type& b) { return a == &b; }   \
  inline bool operator!=(const Type& a, const Type& b) { return !(a == b); } \
  inline bool operator!=(const Type& a, const Type* b) { return !(a == b); } \
  inline bool operator!=(const Type* a, const Type& b) { return !(a == b); }

#define DEFINE_TYPE_CASTS(Type, ArgType, argument, pointerPredicate, referencePredicate)    \
  inline Type* To##Type(ArgType* argument) {                         \
    SECURITY_DCHECK(!argument || (pointerPredicate));                \
    return static_cast<Type*>(argument);                             \
  }                                                                  \
  inline const Type* To##Type(const ArgType* argument) {             \
    SECURITY_DCHECK(!argument || (pointerPredicate));                \
    return static_cast<const Type*>(argument);                       \
  }                                                                  \
  inline Type& To##Type(ArgType& argument) {                         \
    SECURITY_DCHECK(referencePredicate);                             \
    return static_cast<Type&>(argument);                             \
  }                                                                  \
  inline const Type& To##Type(const ArgType& argument) {             \
    SECURITY_DCHECK(referencePredicate);                             \
    return static_cast<const Type&>(argument);                       \
  }                                                                  \
                                                                     \
  inline Type* To##Type##OrNull(ArgType* argument) {                 \
    if (!(argument) || !(pointerPredicate))                          \
      return nullptr;                                                \
    return static_cast<Type*>(argument);                             \
  }                                                                  \
  inline const Type* To##Type##OrNull(const ArgType* argument) {     \
    if (!(argument) || !(pointerPredicate))                          \
      return nullptr;                                                \
    return static_cast<const Type*>(argument);                       \
  }                                                                  \
  inline Type* To##Type##OrNull(ArgType& argument) {                 \
    if (!(referencePredicate))                                       \
      return nullptr;                                                \
    return static_cast<Type*>(&argument);                            \
  }                                                                  \
  inline const Type* To##Type##OrNull(const ArgType& argument) {     \
    if (!(referencePredicate))                                       \
      return nullptr;                                                \
    return static_cast<const Type*>(&argument);                      \
  }                                                                  \

#endif  // BLINKIT_BLINK_ASSERTIONS_H
