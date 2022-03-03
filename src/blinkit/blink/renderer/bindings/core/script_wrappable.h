#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_wrappable.h
// Description: Dispatcher for ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_WRAPPABLE_H
#define BLINKIT_SCRIPT_WRAPPABLE_H

#include "./qjs/script_wrappable_impl.h"

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif // BLINKIT_SCRIPT_WRAPPABLE_H
