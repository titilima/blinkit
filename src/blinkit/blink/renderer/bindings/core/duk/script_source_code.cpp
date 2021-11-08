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

ScriptSourceCode::ScriptSourceCode(const GCRefPtr<ScriptStreamer> &, ScriptResource *resource)
    : m_source(resource->script())
    , m_resource(resource)
{
    TreatNullSourceAsEmpty();
}

std::string ScriptSourceCode::FileName(void) const
{
    if (!m_resource)
        return std::string("inline");

    std::string path = m_resource->url().get_path();
    size_t p = path.rfind('/');
    return std::string::npos == p ? path : path.substr(p + 1);
}

void ScriptSourceCode::TreatNullSourceAsEmpty(void)
{
    if (m_source.isNull())
        m_source = emptyString();
}

} // namespace blink
