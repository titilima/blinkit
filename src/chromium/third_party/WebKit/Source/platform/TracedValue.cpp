// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: TracedValue.cpp
// Description: TracedValue Class
//      Author: Ziming Li
//     Created: 2019-02-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform/TracedValue.h"

#include "base/trace_event/trace_event_argument.h"
#include "wtf/text/StringUTF8Adaptor.h"

namespace blink {

PassRefPtr<TracedValue> TracedValue::create()
{
    return adoptRef(new TracedValue());
}

TracedValue::TracedValue()
{
}

TracedValue::~TracedValue()
{
}

void TracedValue::setInteger(const char* name, int value)
{
    assert(false); // Not reached!
}

void TracedValue::setDouble(const char* name, double value)
{
    assert(false); // Not reached!
}

void TracedValue::setBoolean(const char* name, bool value)
{
    assert(false); // Not reached!
}

void TracedValue::setString(const char* name, const String& value)
{
    assert(false); // Not reached!
}

void TracedValue::beginDictionary(const char* name)
{
    assert(false); // Not reached!
}

void TracedValue::beginArray(const char* name)
{
    assert(false); // Not reached!
}

void TracedValue::endDictionary()
{
    assert(false); // Not reached!
}

void TracedValue::pushInteger(int value)
{
    assert(false); // Not reached!
}

void TracedValue::pushDouble(double value)
{
    assert(false); // Not reached!
}

void TracedValue::pushBoolean(bool value)
{
    assert(false); // Not reached!
}

void TracedValue::pushString(const String& value)
{
    assert(false); // Not reached!
}

void TracedValue::beginArray()
{
    assert(false); // Not reached!
}

void TracedValue::beginDictionary()
{
    assert(false); // Not reached!
}

void TracedValue::endArray()
{
    assert(false); // Not reached!
}

String TracedValue::asTraceFormat() const
{
    assert(false); // Not reached!
    return String();
}

}
