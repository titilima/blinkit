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

class CFunctionEntry final : public JSCFunctionListEntry
{
    STACK_ALLOCATED()
public:
    // JS_CFUNC_DEF
    CFunctionEntry(const char *name, uint8_t length, JSCFunction func1)
    {
        this->name = name;
        this->prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
        this->def_type = JS_DEF_CFUNC;
        this->magic = 0;
        this->u.func.length = length;
        this->u.func.cproto = JS_CFUNC_generic;
        this->u.func.cfunc.generic = func1;
    }
};

class ObjectEntry final : public JSCFunctionListEntry
{
    STACK_ALLOCATED()
public:
    // JS_OBJECT_DEF
    ObjectEntry(const char *name, const JSCFunctionListEntry *tab, int len, uint8_t prop_flags)
    {
        this->name = name;
        this->prop_flags = prop_flags;
        this->def_type = JS_DEF_OBJECT;
        this->magic = 0;
        this->u.prop_list.tab = tab;
        this->u.prop_list.len = len;
    }
};

} // namespace qjs
} // namespace BlinKit


#endif // BLINKIT_QJS_C_FUNCTION_LIST_ENTRIES_H
