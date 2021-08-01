// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_source_code.h
// Description: ScriptSourceCode Class
//      Author: Ziming Li
//     Created: 2018-08-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_SCRIPT_SOURCE_CODE_H
#define BLINKIT_JS_SCRIPT_SOURCE_CODE_H

#pragma once

#include "core/fetch/ResourcePtr.h"
#include "core/fetch/ScriptResource.h"
#include "platform/weborigin/KURL.h"
#include "wtf/text/TextPosition.h"
#include "wtf/text/WTFString.h"
#include "script_streamer.h"

namespace blink {

class ScriptSourceCode final
{
public:
    explicit ScriptSourceCode(ScriptResource *);
    ScriptSourceCode(const String &, const KURL & = KURL(),
        const TextPosition &startPosition = TextPosition::minimumPosition());
    ScriptSourceCode(PassRefPtrWillBeRawPtr<ScriptStreamer>, ScriptResource*);

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

#endif // BLINKIT_JS_SCRIPT_SOURCE_CODE_H
