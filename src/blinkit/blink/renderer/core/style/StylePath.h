// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StylePath.h
// Description: StylePath Class
//      Author: Ziming Li
//     Created: 2021-07-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef StylePath_h
#define StylePath_h

#include "platform/graphics/Path.h"
#include "platform/heap/Handle.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/RefPtr.h"

namespace blink {

class CSSValue;
class SVGPathByteStream;

class StylePath : public RefCounted<StylePath> {
public:
    // BKTODO: static PassRefPtr<StylePath> create(PassRefPtr<SVGPathByteStream>);
    ~StylePath();

    static StylePath* emptyPath();

    const Path& path() const { return m_path; }
    // BKTODO: const SVGPathByteStream& byteStream() const;

    PassRefPtrWillBeRawPtr<CSSValue> computedCSSValue() const;

    bool equals(const StylePath&) const;

private:
#if 0 // BKTODO:
    explicit StylePath(PassRefPtr<SVGPathByteStream>);

    RefPtr<SVGPathByteStream> m_byteStream;
#endif
    Path m_path;
};

} // namespace blink

#endif // StylePath_h
