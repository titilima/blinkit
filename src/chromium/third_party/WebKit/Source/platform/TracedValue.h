// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: TracedValue.h
// Description: TracedValue Class
//      Author: Ziming Li
//     Created: 2019-02-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TracedValue_h
#define TracedValue_h

#include "platform/EventTracer.h"
#include "wtf/PassRefPtr.h"
#include "wtf/text/WTFString.h"

namespace blink {

class PLATFORM_EXPORT TracedValue : public TraceEvent::ConvertableToTraceFormat {
    WTF_MAKE_NONCOPYABLE(TracedValue);

public:
    static PassRefPtr<TracedValue> create();

    void endDictionary();
    void endArray();

    void setInteger(const char* name, int value);
    void setDouble(const char* name, double);
    void setBoolean(const char* name, bool value);
    void setString(const char* name, const String& value);
    void beginArray(const char* name);
    void beginDictionary(const char* name);

    void pushInteger(int);
    void pushDouble(double);
    void pushBoolean(bool);
    void pushString(const String&);
    void beginArray();
    void beginDictionary();

    String asTraceFormat() const override;

private:
    TracedValue();
    ~TracedValue() override;
};

} // namespace blink

#endif // TracedValue_h
