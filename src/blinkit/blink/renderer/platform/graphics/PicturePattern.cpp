// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PicturePattern.cpp
// Description: PicturePattern Class
//      Author: Ziming Li
//     Created: 2022-02-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./PicturePattern.h"

#include "blinkit/blink/renderer/platform/graphics/skia/skia_utils.h"
#include "third_party/skia/include/core/SkPicture.h"
#include "third_party/skia/include/core/SkShader.h"

namespace blink {

PassRefPtr<PicturePattern> PicturePattern::create(PassRefPtr<const SkPicture> picture,
    RepeatMode repeatMode)
{
    return adoptRef(new PicturePattern(picture, repeatMode));
}

PicturePattern::PicturePattern(PassRefPtr<const SkPicture> picture, RepeatMode mode)
    : Pattern(mode)
    , m_tilePicture(picture)
{
    // All current clients use RepeatModeXY, so we only support this mode for now.
    ASSERT(isRepeatXY());

    // FIXME: we don't have a good way to account for DL memory utilization.
}

PicturePattern::~PicturePattern()
{
}

PassRefPtr<SkShader> PicturePattern::createShader()
{
    SkMatrix localMatrix = affineTransformToSkMatrix(m_patternSpaceTransformation);
    SkRect tileBounds = m_tilePicture->cullRect();

    return adoptRef(SkShader::CreatePictureShader(m_tilePicture.get(),
        SkShader::kRepeat_TileMode, SkShader::kRepeat_TileMode, &localMatrix, &tileBounds));
}

} // namespace
