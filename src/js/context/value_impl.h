// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: value_impl.h
// Description: ValueImpl Class
//      Author: Ziming Li
//     Created: 2019-05-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_VALUE_IMPL_H
#define BLINKIT_JS_VALUE_IMPL_H

#pragma once

namespace BlinKit {

class ValueImpl final : public BkValue
{
public:
    ValueImpl(duk_context *ctx, duk_idx_t idx = -1);

    void SetAsErrorType(void);
    int ErrorCode(void) const { return m_errorCode; }
private:
    Type BKAPI GetType(void) const override;
    bool BKAPI GetAsBoolean(void) const override;
    int BKAPI GetAsString(BkBuffer &dst) const override;
    int BKAPI GetAsJSON(BkBuffer &dst) const override;

    duk_context *m_ctx;
    duk_idx_t m_idx;
    int m_errorCode = BkError::Success;
};

} // namespace BlinKit

#endif // BLINKIT_JS_VALUE_IMPL_H
