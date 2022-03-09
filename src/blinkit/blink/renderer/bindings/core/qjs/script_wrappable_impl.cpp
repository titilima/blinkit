// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_wrappable_impl.cpp
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./script_wrappable_impl.h"

#include "blinkit/js/runtime.h"

namespace blink {

static JSClassID g_scriptWrappableClassId = 0;
static const char ClassName[] = "ScriptWrappable";

ScriptWrappable::ScriptWrappable(void) : m_scriptObject(JS_UNINITIALIZED)
{
}

ScriptWrappable::~ScriptWrappable(void)
{
    JS_FreeValueRT(g_runtime, m_scriptObject);
}

ScriptWrappable* ScriptWrappable::CastImpl(JSValue v)
{
    return reinterpret_cast<ScriptWrappable *>(JS_GetOpaque(v, g_scriptWrappableClassId));
}

void ScriptWrappable::Initialize(void)
{
    JS_NewClassID(&g_scriptWrappableClassId);

    JSClassDef def = { 0 };
    def.class_name = ClassName;
    JS_NewClass(g_runtime, g_scriptWrappableClassId, &def);
}

JSValue ScriptWrappable::NewObject(JSContext *ctx, JSValueConst proto, ScriptWrappable *nativeObject)
{
    ASSERT(JS_UNINITIALIZED == nativeObject->m_scriptObject);
    nativeObject->m_scriptObject = JS_NewObjectProtoClass(ctx, proto, g_scriptWrappableClassId);
    JS_SetOpaque(nativeObject->m_scriptObject, nativeObject);
    return nativeObject->m_scriptObject;
}

} // namespace blink
