// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: open_type_vertical_data.cc
// Description: OpenTypeVerticalData Class
//      Author: Ziming Li
//     Created: 2020-12-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Koji Ishii <kojiishi@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/platform/fonts/opentype/open_type_vertical_data.h"

#include "SkTypeface.h"
#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/platform/fonts/opentype/open_type_types.h"
#include "third_party/blink/renderer/platform/fonts/simple_font_data.h"
#include "third_party/blink/renderer/platform/fonts/skia/skia_text_metrics.h"
#include "third_party/blink/renderer/platform/geometry/float_rect.h"
#include "third_party/blink/renderer/platform/wtf/std_lib_extras.h"

namespace blink {
namespace OpenType {

// The input characters are big-endian (first is most significant).
#define OT_MAKE_TAG(ch1, ch2, ch3, ch4)                    \
  ((((uint32_t)(ch1)) << 24) | (((uint32_t)(ch2)) << 16) | \
   (((uint32_t)(ch3)) << 8) | ((uint32_t)(ch4)))

const SkFontTableTag kHheaTag = OT_MAKE_TAG('h', 'h', 'e', 'a');
const SkFontTableTag kHmtxTag = OT_MAKE_TAG('h', 'm', 't', 'x');
const SkFontTableTag kVheaTag = OT_MAKE_TAG('v', 'h', 'e', 'a');
const SkFontTableTag kVmtxTag = OT_MAKE_TAG('v', 'm', 't', 'x');
const SkFontTableTag kVORGTag = OT_MAKE_TAG('V', 'O', 'R', 'G');

#pragma pack(1)

struct HheaTable {
  DISALLOW_NEW();
  OpenType::Fixed version;
  OpenType::Int16 ascender;
  OpenType::Int16 descender;
  OpenType::Int16 line_gap;
  OpenType::Int16 advance_width_max;
  OpenType::Int16 min_left_side_bearing;
  OpenType::Int16 min_right_side_bearing;
  OpenType::Int16 x_max_extent;
  OpenType::Int16 caret_slope_rise;
  OpenType::Int16 caret_slope_run;
  OpenType::Int16 caret_offset;
  OpenType::Int16 reserved[4];
  OpenType::Int16 metric_data_format;
  OpenType::UInt16 number_of_h_metrics;
};

struct VheaTable {
  DISALLOW_NEW();
  OpenType::Fixed version;
  OpenType::Int16 ascent;
  OpenType::Int16 descent;
  OpenType::Int16 line_gap;
  OpenType::Int16 advance_height_max;
  OpenType::Int16 min_top_side_bearing;
  OpenType::Int16 min_bottom_side_bearing;
  OpenType::Int16 y_max_extent;
  OpenType::Int16 caret_slope_rise;
  OpenType::Int16 caret_slope_run;
  OpenType::Int16 caret_offset;
  OpenType::Int16 reserved[4];
  OpenType::Int16 metric_data_format;
  OpenType::UInt16 num_of_long_ver_metrics;
};

struct HmtxTable {
  DISALLOW_NEW();
  struct Entry {
    DISALLOW_NEW();
    OpenType::UInt16 advance_width;
    OpenType::Int16 lsb;
  } entries[1];
};

struct VmtxTable {
  DISALLOW_NEW();
  struct Entry {
    DISALLOW_NEW();
    OpenType::UInt16 advance_height;
    OpenType::Int16 top_side_bearing;
  } entries[1];
};

struct VORGTable {
  DISALLOW_NEW();
  OpenType::UInt16 major_version;
  OpenType::UInt16 minor_version;
  OpenType::Int16 default_vert_origin_y;
  OpenType::UInt16 num_vert_origin_y_metrics;
  struct VertOriginYMetrics {
    DISALLOW_NEW();
    OpenType::UInt16 glyph_index;
    OpenType::Int16 vert_origin_y;
  } vert_origin_y_metrics[1];

  size_t RequiredSize() const {
    return sizeof(*this) +
           sizeof(VertOriginYMetrics) * (num_vert_origin_y_metrics - 1);
  }
};

#pragma pack()

}  // namespace OpenType

OpenTypeVerticalData::OpenTypeVerticalData(sk_sp<SkTypeface> typeface)
    : default_vert_origin_y_(0),
      size_per_unit_(0),
      ascent_fallback_(0),
      height_fallback_(0) {
  LoadMetrics(typeface);
}

static void CopyOpenTypeTable(sk_sp<SkTypeface> typeface,
                              SkFontTableTag tag,
                              Vector<char>& destination) {
  const size_t table_size = typeface->getTableSize(tag);
  destination.resize(SafeCast<wtf_size_t>(table_size));
  if (table_size) {
    typeface->getTableData(tag, 0, table_size, destination.data());
  }
}

void OpenTypeVerticalData::LoadMetrics(sk_sp<SkTypeface> typeface) {
  // Load hhea and hmtx to get x-component of vertical origins.
  // If these tables are missing, it's not an OpenType font.
  Vector<char> buffer;
  CopyOpenTypeTable(typeface, OpenType::kHheaTag, buffer);
  const OpenType::HheaTable* hhea =
      OpenType::ValidateTable<OpenType::HheaTable>(buffer);
  if (!hhea)
    return;
  uint16_t count_hmtx_entries = hhea->number_of_h_metrics;
  if (!count_hmtx_entries) {
    BKLOG("ERROR: Invalid numberOfHMetrics.");
    return;
  }

  CopyOpenTypeTable(typeface, OpenType::kHmtxTag, buffer);
  const OpenType::HmtxTable* hmtx =
      OpenType::ValidateTable<OpenType::HmtxTable>(buffer, count_hmtx_entries);
  if (!hmtx) {
    BKLOG("ERROR: hhea exists but hmtx does not (or broken).");
    return;
  }
  advance_widths_.resize(count_hmtx_entries);
  for (uint16_t i = 0; i < count_hmtx_entries; ++i)
    advance_widths_[i] = hmtx->entries[i].advance_width;

  // Load vhea first. This table is required for fonts that support vertical
  // flow.
  CopyOpenTypeTable(typeface, OpenType::kVheaTag, buffer);
  const OpenType::VheaTable* vhea =
      OpenType::ValidateTable<OpenType::VheaTable>(buffer);
  if (!vhea)
    return;
  uint16_t count_vmtx_entries = vhea->num_of_long_ver_metrics;
  if (!count_vmtx_entries) {
    BKLOG("ERROR: Invalid numOfLongVerMetrics");
    return;
  }

  // Load VORG. This table is optional.
  CopyOpenTypeTable(typeface, OpenType::kVORGTag, buffer);
  const OpenType::VORGTable* vorg =
      OpenType::ValidateTable<OpenType::VORGTable>(buffer);
  if (vorg && buffer.size() >= vorg->RequiredSize()) {
    default_vert_origin_y_ = vorg->default_vert_origin_y;
    uint16_t count_vert_origin_y_metrics = vorg->num_vert_origin_y_metrics;
    if (!count_vert_origin_y_metrics) {
      // Add one entry so that hasVORG() becomes true
      vert_origin_y_.Set(0, default_vert_origin_y_);
    } else {
      for (uint16_t i = 0; i < count_vert_origin_y_metrics; ++i) {
        const OpenType::VORGTable::VertOriginYMetrics& metrics =
            vorg->vert_origin_y_metrics[i];
        vert_origin_y_.Set(metrics.glyph_index, metrics.vert_origin_y);
      }
    }
  }

  // Load vmtx then. This table is required for fonts that support vertical
  // flow.
  CopyOpenTypeTable(typeface, OpenType::kVmtxTag, buffer);
  const OpenType::VmtxTable* vmtx =
      OpenType::ValidateTable<OpenType::VmtxTable>(buffer, count_vmtx_entries);
  if (!vmtx) {
    BKLOG("ERROR: vhea exists but vmtx does not (or broken)");
    return;
  }
  advance_heights_.resize(count_vmtx_entries);
  for (uint16_t i = 0; i < count_vmtx_entries; ++i)
    advance_heights_[i] = vmtx->entries[i].advance_height;

  // VORG is preferred way to calculate vertical origin than vmtx,
  // so load topSideBearing from vmtx only if VORG is missing.
  if (HasVORG())
    return;

  wtf_size_t size_extra =
      buffer.size() - sizeof(OpenType::VmtxTable::Entry) * count_vmtx_entries;
  if (size_extra % sizeof(OpenType::Int16)) {
    BKLOG("ERROR: vmtx has incorrect tsb count");
    return;
  }
  wtf_size_t count_top_side_bearings =
      count_vmtx_entries + size_extra / sizeof(OpenType::Int16);
  top_side_bearings_.resize(count_top_side_bearings);
  wtf_size_t i;
  for (i = 0; i < count_vmtx_entries; ++i)
    top_side_bearings_[i] = vmtx->entries[i].top_side_bearing;
  if (i < count_top_side_bearings) {
    const OpenType::Int16* p_top_side_bearings_extra =
        reinterpret_cast<const OpenType::Int16*>(
            &vmtx->entries[count_vmtx_entries]);
    for (; i < count_top_side_bearings; ++i, ++p_top_side_bearings_extra)
      top_side_bearings_[i] = *p_top_side_bearings_extra;
  }
}

void OpenTypeVerticalData::SetScaleAndFallbackMetrics(float size_per_unit,
                                                      float ascent,
                                                      int height) {
  size_per_unit_ = size_per_unit;
  ascent_fallback_ = ascent;
  height_fallback_ = height;
}

float OpenTypeVerticalData::AdvanceHeight(Glyph glyph) const {
  wtf_size_t count_heights = advance_heights_.size();
  if (count_heights) {
    uint16_t advance_f_unit =
        advance_heights_[glyph < count_heights ? glyph : count_heights - 1];
    float advance = advance_f_unit * size_per_unit_;
    return advance;
  }

  // No vertical info in the font file; use height as advance.
  return height_fallback_;
}

void OpenTypeVerticalData::GetVerticalTranslationsForGlyphs(
    const SkPaint& paint,
    const Glyph* glyphs,
    size_t count,
    float* out_xy_array) const {
  wtf_size_t count_widths = advance_widths_.size();
  DCHECK_GT(count_widths, 0u);
  bool use_vorg = HasVORG();
  wtf_size_t count_top_side_bearings = top_side_bearings_.size();
  float default_vert_origin_y = std::numeric_limits<float>::quiet_NaN();
  for (float *end = &(out_xy_array[count * 2]); out_xy_array != end;
       ++glyphs, out_xy_array += 2) {
    Glyph glyph = *glyphs;
    uint16_t width_f_unit =
        advance_widths_[glyph < count_widths ? glyph : count_widths - 1];
    float width = width_f_unit * size_per_unit_;
    out_xy_array[0] = -width / 2;

    // For Y, try VORG first.
    if (use_vorg) {
      if (glyph) {
        int16_t vert_origin_yf_unit = vert_origin_y_.at(glyph);
        if (vert_origin_yf_unit) {
          out_xy_array[1] = -vert_origin_yf_unit * size_per_unit_;
          continue;
        }
      }
      if (std::isnan(default_vert_origin_y))
        default_vert_origin_y = -default_vert_origin_y_ * size_per_unit_;
      out_xy_array[1] = default_vert_origin_y;
      continue;
    }

    // If no VORG, try vmtx next.
    if (count_top_side_bearings) {
      int16_t top_side_bearing_f_unit =
          top_side_bearings_[glyph < count_top_side_bearings
                                 ? glyph
                                 : count_top_side_bearings - 1];
      float top_side_bearing = top_side_bearing_f_unit * size_per_unit_;

      SkRect skiaBounds;
      SkiaTextMetrics(&paint).GetSkiaBoundsForGlyph(glyph, &skiaBounds);
      FloatRect bounds(skiaBounds);
      out_xy_array[1] = bounds.Y() - top_side_bearing;
      continue;
    }

    // No vertical info in the font file; use ascent as vertical origin.
    out_xy_array[1] = -ascent_fallback_;
  }
}

}  // namespace blink
