#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_source_code.h
// Description: ScriptSourceCode Class
//      Author: Ziming Li
//     Created: 2018-08-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_SOURCE_CODE_H
#define BLINKIT_SCRIPT_SOURCE_CODE_H

#include "blinkit/blink/renderer/bindings/core/script_streamer.h"
#include "blinkit/blink/renderer/core/fetch/ResourcePtr.h"
#include "blinkit/blink/renderer/core/fetch/ScriptResource.h"
#include "blinkit/blink/renderer/platform/weborigin/KURL.h"
#include "blinkit/blink/renderer/wtf/text/TextPosition.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class ScriptSourceCode final
{
public:
    explicit ScriptSourceCode(ScriptResource *);
    ScriptSourceCode(const String &, const KURL & = KURL(),
        const TextPosition &startPosition = TextPosition::minimumPosition());
    ScriptSourceCode(const GCRefPtr<ScriptStreamer> &, ScriptResource *);

    bool isEmpty(void) const { return m_source.isEmpty(); }
    const String& source(void) const { return m_source; }

    ScriptResource* resource(void) const { return m_resource.get(); }

    std::string FileName(void) const;
private:
    void TreatNullSourceAsEmpty(void);

    String m_source;
    ResourcePtr<ScriptResource> m_resource;
};

} // namespace blink

#endif // BLINKIT_SCRIPT_SOURCE_CODE_H
