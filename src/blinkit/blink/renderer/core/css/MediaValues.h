// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaValues.h
// Description: MediaValues Class
//      Author: Ziming Li
//     Created: 2021-09-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MediaValues_h
#define MediaValues_h

#include "core/CoreExport.h"
#include "core/css/CSSPrimitiveValue.h"
#include "public/platform/PointerProperties.h"
#include "public/platform/WebDisplayMode.h"
#include "wtf/RefCounted.h"
#include "wtf/RefPtr.h"

namespace blink {

class Document;
class CSSPrimitiveValue;
class LocalFrame;

class CORE_EXPORT MediaValues {
public:

    virtual ~MediaValues() { }

    static std::shared_ptr<MediaValues> createDynamicIfFrameExists(LocalFrame*);
    virtual std::shared_ptr<MediaValues> copy() const = 0;
    virtual bool isSafeToSendToAnotherThread() const = 0;

    static bool computeLengthImpl(double value, CSSPrimitiveValue::UnitType, unsigned defaultFontSize, double viewportWidth, double viewportHeight, double& result);
    template<typename T>
    static bool computeLength(double value, CSSPrimitiveValue::UnitType type, unsigned defaultFontSize, double viewportWidth, double viewportHeight, T& result)
    {
        double tempResult;
        if (!computeLengthImpl(value, type, defaultFontSize, viewportWidth, viewportHeight, tempResult))
            return false;
        result = clampTo<T>(tempResult);
        return true;
    }
    virtual bool computeLength(double value, CSSPrimitiveValue::UnitType, int& result) const = 0;
    virtual bool computeLength(double value, CSSPrimitiveValue::UnitType, double& result) const = 0;

    virtual double viewportWidth() const = 0;
    virtual double viewportHeight() const = 0;
    virtual int deviceWidth() const = 0;
    virtual int deviceHeight() const = 0;
    virtual float devicePixelRatio() const = 0;
    virtual int colorBitsPerComponent() const = 0;
    virtual int monochromeBitsPerComponent() const = 0;
    virtual PointerType primaryPointerType() const = 0;
    virtual int availablePointerTypes() const = 0;
    virtual HoverType primaryHoverType() const = 0;
    virtual int availableHoverTypes() const = 0;
    virtual bool threeDEnabled() const = 0;
    virtual const String mediaType() const = 0;
    virtual WebDisplayMode displayMode() const = 0;
    virtual bool strictMode() const = 0;
    virtual Document* document() const = 0;
    virtual bool hasValues() const = 0;

    virtual bool isCached() const { return false; }

protected:
    double calculateViewportWidth(LocalFrame*) const;
    double calculateViewportHeight(LocalFrame*) const;
    int calculateDeviceWidth(LocalFrame*) const;
    int calculateDeviceHeight(LocalFrame*) const;
    bool calculateStrictMode(LocalFrame*) const;
    float calculateDevicePixelRatio(LocalFrame*) const;
    int calculateColorBitsPerComponent(LocalFrame*) const;
    int calculateMonochromeBitsPerComponent(LocalFrame*) const;
    int calculateDefaultFontSize(LocalFrame*) const;
    const String calculateMediaType(LocalFrame*) const;
    WebDisplayMode calculateDisplayMode(LocalFrame*) const;
    bool calculateThreeDEnabled(LocalFrame*) const;
    PointerType calculatePrimaryPointerType(LocalFrame*) const;
    int calculateAvailablePointerTypes(LocalFrame*) const;
    HoverType calculatePrimaryHoverType(LocalFrame*) const;
    int calculateAvailableHoverTypes(LocalFrame*) const;
    static LocalFrame* frameFrom(Document&);

};

} // namespace

#endif // MediaValues_h
