// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: function_context_impl.h
// Description: FunctionContextImpl Class
//      Author: Ziming Li
//     Created: 2019-05-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_FUNCTION_CONTEXT_IMPL_H
#define BLINKIT_JS_FUNCTION_CONTEXT_IMPL_H

#pragma once

#include <vector>

namespace BlinKit {

class ValueImpl;

class FunctionContextImpl final : public BkFunction::Context
{
public:
    FunctionContextImpl(duk_context *ctx, const std::string &name);
    ~FunctionContextImpl(void);

    bool HasReturnValue(void) const { return m_hasReturnValue; }
private:
    const char* BKAPI Name(void) const override { return m_name.c_str(); }
    size_t BKAPI ArgCount(void) const override;
    const BkValue* BKAPI ArgAt(size_t i) const override;
    int BKAPI ReturnAsBoolean(bool b) override;
    int BKAPI ReturnAsJSON(const char *json, size_t length) override;

    duk_context *m_ctx;
    const std::string &m_name;
    std::vector<ValueImpl> m_args;
    bool m_hasReturnValue = false;
};

} // namespace BlinKit

#endif // BLINKIT_JS_FUNCTION_CONTEXT_IMPL_H
