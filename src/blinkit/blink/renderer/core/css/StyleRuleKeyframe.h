// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleRuleKeyframe.h
// Description: StyleRuleKeyframe Class
//      Author: Ziming Li
//     Created: 2021-10-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef StyleRuleKeyframe_h
#define StyleRuleKeyframe_h

#include "core/css/StyleRule.h"

namespace blink {

class MutableStylePropertySet;
class StylePropertySet;

class StyleRuleKeyframe final : public StyleRuleBase {
    USING_FAST_MALLOC_WILL_BE_REMOVED(StyleRuleKeyframe);
public:
    static GCRefPtr<StyleRuleKeyframe> create(PassOwnPtr<Vector<double>> keys, const GCRefPtr<StylePropertySet> &properties)
    {
        return BlinKit::GCWrapShared(new StyleRuleKeyframe(keys, properties));
    }

    // Exposed to JavaScript.
    String keyText() const;
    bool setKeyText(const String&);

    // Used by StyleResolver.
    const Vector<double>& keys() const;

    const StylePropertySet& properties() const { return *m_properties; }
    MutableStylePropertySet& mutableProperties();

    String cssText() const;

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    StyleRuleKeyframe(PassOwnPtr<Vector<double>>, const GCRefPtr<StylePropertySet> &);

    GCRefPtr<StylePropertySet> m_properties;
    Vector<double> m_keys;
};

DEFINE_STYLE_RULE_TYPE_CASTS(Keyframe);

} // namespace blink

#endif // StyleRuleKeyframe_h
