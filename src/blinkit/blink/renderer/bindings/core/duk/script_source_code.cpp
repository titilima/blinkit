// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_source_code.cpp
// Description: ScriptSourceCode Class
//      Author: Ziming Li
//     Created: 2018-08-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "./script_source_code.h"

namespace blink {

ScriptSourceCode::ScriptSourceCode(const String &source, const KURL &url, const TextPosition &startPosition)
    : m_source(source)
{
    TreatNullSourceAsEmpty();
}

ScriptSourceCode::ScriptSourceCode(ScriptResource *resource)
    : m_source(resource->script())
    , m_resource(resource)
{
    TreatNullSourceAsEmpty();
}

ScriptSourceCode::ScriptSourceCode(PassRefPtrWillBeRawPtr<ScriptStreamer> streamer, ScriptResource *resource)
    : m_source(resource->script())
    , m_resource(resource)
{
    TreatNullSourceAsEmpty();
}

std::string ScriptSourceCode::FileName(void) const
{
    ASSERT(false); // BKTODO:
    return std::string();
#if 0
    if (m_resource)
        return m_resource->url().lastPathComponent().to_string();
    else
        return std::string("inline");
#endif
}

void ScriptSourceCode::TreatNullSourceAsEmpty(void)
{
    if (m_source.isNull())
        m_source = "";
}

} // namespace blink
