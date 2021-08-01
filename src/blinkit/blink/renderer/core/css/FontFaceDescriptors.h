// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FontFaceDescriptors.h
// Description: FontFaceDescriptors Class
//      Author: Ziming Li
//     Created: 2018-07-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_FONT_FACE_DESCRIPTORS_H
#define BLINKIT_BLINKIT_FONT_FACE_DESCRIPTORS_H

#pragma once

#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class FontFaceDescriptors
{
public:
    bool hasFeatureSettings(void) const { return !m_featureSettings.isNull(); }
    const String& featureSettings(void) const { return m_featureSettings; }
    void setFeatureSettings(const String &value) { m_featureSettings = value; }

    bool hasStretch(void) const { return !m_stretch.isNull(); }
    const String& stretch(void) const { return m_stretch; }
    void setStretch(const String &value) { m_stretch = value; }

    bool hasStyle(void) const { return !m_style.isNull(); }
    const String& style(void) const { return m_style; }
    void setStyle(const String &value) { m_style = value; }

    bool hasUnicodeRange(void) const { return !m_unicodeRange.isNull(); }
    const String& unicodeRange(void) const { return m_unicodeRange; }
    void setUnicodeRange(const String &value) { m_unicodeRange = value; }

    bool hasVariant(void) const { return !m_variant.isNull(); }
    const String& variant(void) const { return m_variant; }
    void setVariant(const String &value) { m_variant = value; }

    bool hasWeight(void) const { return !m_weight.isNull(); }
    const String& weight(void) const { return m_weight; }
    void setWeight(const String &value) { m_weight = value; }
private:
    String m_featureSettings{ "normal" };
    String m_stretch{ "normal" };
    String m_style{ "normal" };
    String m_unicodeRange{ "U+0-10FFFF" };
    String m_variant{ "normal" };
    String m_weight{ "normal" };
};

} // namespace blink

#endif // BLINKIT_BLINKIT_FONT_FACE_DESCRIPTORS_H
