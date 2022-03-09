#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_c_function_list_entries.h
// Description: JSCFunctionListEntry Wrapper Classes
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QJS_C_FUNCTION_LIST_ENTRIES_H
#define BLINKIT_QJS_C_FUNCTION_LIST_ENTRIES_H

#include "third_party/quickjs/quickjs.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace BlinKit {
namespace qjs {

class CFunctionListEntry : public JSCFunctionListEntry
{
protected:
    CFunctionListEntry(const char *name, uint8_t propFlags, uint8_t defType, int16_t magic = 0)
    {
        this->name = name;
        this->prop_flags = propFlags;
        this->def_type = defType;
        this->magic = magic;
    }
};

// JS_CFUNC_DEF
class CFunctionEntry final : public CFunctionListEntry
{
    STACK_ALLOCATED()
public:
    CFunctionEntry(const char *name, uint8_t length, JSCFunction func1, int16_t magic = 0)
        : CFunctionListEntry(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, JS_DEF_CFUNC, magic)
    {
        this->u.func.length = length;
        this->u.func.cproto = JS_CFUNC_generic;
        this->u.func.cfunc.generic = func1;
    }
};

// JS_CGETSET_DEF
class CGetSetEntry final : public CFunctionListEntry
{
    STACK_ALLOCATED()
public:
    using Getter = JSValue (*)(JSContext *, JSValueConst);
    using Setter = JSValue (*)(JSContext *, JSValueConst, JSValueConst);

    CGetSetEntry(const char *name, Getter fgetter, Setter fsetter, int16_t magic = 0)
        : CFunctionListEntry(name, JS_PROP_CONFIGURABLE, JS_DEF_CGETSET, magic)
    {
        this->u.getset.get.getter = fgetter;
        this->u.getset.set.setter = fsetter;
    }
};

// JS_OBJECT_DEF
class ObjectEntry final : public CFunctionListEntry
{
    STACK_ALLOCATED()
public:
    ObjectEntry(const char *name, const JSCFunctionListEntry *tab, int len, uint8_t prop_flags)
        : CFunctionListEntry(name, prop_flags, JS_DEF_OBJECT)
    {
        this->u.prop_list = { tab, len };
    }
};

// JS_ALIAS_DEF
class AliasEntry final : public CFunctionListEntry
{
    STACK_ALLOCATED()
public:
    AliasEntry(const char *name, const char *from)
        : CFunctionListEntry(name, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, JS_DEF_ALIAS)
    {
        this->u.alias = { from, -1 };
    }
};

} // namespace qjs
} // namespace BlinKit


#endif // BLINKIT_QJS_C_FUNCTION_LIST_ENTRIES_H
