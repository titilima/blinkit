// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ShadowRootInit.h
// Description: ShadowRootInit Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SHADOW_ROOT_INIT_H
#define BLINKIT_BLINK_SHADOW_ROOT_INIT_H

#pragma once

#include <optional>
#include "wtf/text/WTFString.h"

namespace blink {

class ShadowRootInit
{
public:
    bool hasDelegatesFocus(void) const { return m_delegatesFocus.has_value(); }
    bool delegatesFocus(void) const { return m_delegatesFocus.value(); }

    bool hasMode(void) const { return !m_mode.isNull(); }
    const String& mode(void) const { return m_mode; }
private:
    std::optional<bool> m_delegatesFocus;
    String m_mode;
};

} // namespace blink

#endif // BLINKIT_BLINK_SHADOW_ROOT_INIT_H
