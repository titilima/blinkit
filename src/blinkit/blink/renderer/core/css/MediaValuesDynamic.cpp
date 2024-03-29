// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaValuesDynamic.cpp
// Description: MediaValuesDynamic Class
//      Author: Ziming Li
//     Created: 2021-09-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/MediaValuesDynamic.h"

#include "core/css/CSSHelper.h"
#include "core/css/CSSPrimitiveValue.h"
#include "core/css/CSSToLengthConversionData.h"
#include "core/css/MediaValuesCached.h"
#include "core/dom/Document.h"
#include "core/frame/LocalFrame.h"

namespace blink {

std::shared_ptr<MediaValues> MediaValuesDynamic::create(Document& document)
{
    return MediaValuesDynamic::create(frameFrom(document));
}

std::shared_ptr<MediaValues> MediaValuesDynamic::create(LocalFrame* frame)
{
    if (!frame || !frame->view() || !frame->document() || !frame->document()->layoutView())
        return MediaValuesCached::create();
    return zed::wrap_shared(new MediaValuesDynamic(frame));
}

MediaValuesDynamic::MediaValuesDynamic(LocalFrame* frame)
    : m_frame(frame)
{
    ASSERT(m_frame);
}

std::shared_ptr<MediaValues> MediaValuesDynamic::copy() const
{
    return zed::wrap_shared(new MediaValuesDynamic(m_frame));
}

bool MediaValuesDynamic::computeLength(double value, CSSPrimitiveValue::UnitType type, int& result) const
{
    return MediaValues::computeLength(value,
        type,
        calculateDefaultFontSize(m_frame),
        calculateViewportWidth(m_frame),
        calculateViewportHeight(m_frame),
        result);
}

bool MediaValuesDynamic::computeLength(double value, CSSPrimitiveValue::UnitType type, double& result) const
{
    return MediaValues::computeLength(value,
        type,
        calculateDefaultFontSize(m_frame),
        calculateViewportWidth(m_frame),
        calculateViewportHeight(m_frame),
        result);
}

bool MediaValuesDynamic::isSafeToSendToAnotherThread() const
{
    return false;
}

double MediaValuesDynamic::viewportWidth() const
{
    return calculateViewportWidth(m_frame);
}

double MediaValuesDynamic::viewportHeight() const
{
    return calculateViewportHeight(m_frame);
}

int MediaValuesDynamic::deviceWidth() const
{
    return calculateDeviceWidth(m_frame);
}

int MediaValuesDynamic::deviceHeight() const
{
    return calculateDeviceHeight(m_frame);
}

float MediaValuesDynamic::devicePixelRatio() const
{
    return calculateDevicePixelRatio(m_frame);
}

int MediaValuesDynamic::colorBitsPerComponent() const
{
    return calculateColorBitsPerComponent(m_frame);
}

int MediaValuesDynamic::monochromeBitsPerComponent() const
{
    return calculateMonochromeBitsPerComponent(m_frame);
}

PointerType MediaValuesDynamic::primaryPointerType() const
{
    return calculatePrimaryPointerType(m_frame);
}

int MediaValuesDynamic::availablePointerTypes() const
{
    return calculateAvailablePointerTypes(m_frame);
}

HoverType MediaValuesDynamic::primaryHoverType() const
{
    return calculatePrimaryHoverType(m_frame);
}

int MediaValuesDynamic::availableHoverTypes() const
{
    return calculateAvailableHoverTypes(m_frame);
}

bool MediaValuesDynamic::threeDEnabled() const
{
    return calculateThreeDEnabled(m_frame);
}

const String MediaValuesDynamic::mediaType() const
{
    return calculateMediaType(m_frame);
}

WebDisplayMode MediaValuesDynamic::displayMode() const
{
    return calculateDisplayMode(m_frame);
}

bool MediaValuesDynamic::strictMode() const
{
    return calculateStrictMode(m_frame);
}

Document* MediaValuesDynamic::document() const
{
    return m_frame->document();
}

bool MediaValuesDynamic::hasValues() const
{
    return m_frame;
}

} // namespace
