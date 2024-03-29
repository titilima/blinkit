// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaValuesDynamic.h
// Description: MediaValuesDynamic Class
//      Author: Ziming Li
//     Created: 2021-09-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MediaValuesDynamic_h
#define MediaValuesDynamic_h

#include "core/css/MediaValues.h"

namespace blink {

class Document;

class MediaValuesDynamic final : public MediaValues {
public:
    static std::shared_ptr<MediaValues> create(Document&);
    static std::shared_ptr<MediaValues> create(LocalFrame*);
    std::shared_ptr<MediaValues> copy() const override;
    bool isSafeToSendToAnotherThread() const override;
    bool computeLength(double value, CSSPrimitiveValue::UnitType, int& result) const override;
    bool computeLength(double value, CSSPrimitiveValue::UnitType, double& result) const override;

    double viewportWidth() const override;
    double viewportHeight() const override;
    int deviceWidth() const override;
    int deviceHeight() const override;
    float devicePixelRatio() const override;
    int colorBitsPerComponent() const override;
    int monochromeBitsPerComponent() const override;
    PointerType primaryPointerType() const override;
    int availablePointerTypes() const override;
    HoverType primaryHoverType() const override;
    int availableHoverTypes() const override;
    bool threeDEnabled() const override;
    bool strictMode() const override;
    const String mediaType() const override;
    WebDisplayMode displayMode() const override;
    Document* document() const override;
    bool hasValues() const override;

protected:
    MediaValuesDynamic(LocalFrame*);

    // This raw ptr is safe, as MediaValues would not outlive MediaQueryEvaluator, and
    // MediaQueryEvaluator is reset on |Document::detach|.
    LocalFrame *m_frame;
};

} // namespace

#endif // MediaValuesDynamic_h
