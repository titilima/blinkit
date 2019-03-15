// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_theme_engine.h
// Description: WinThemeEngine Class
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
#define BLINKIT_BLINKIT_WIN_THEME_ENGINE_H

#pragma once

#include "public/platform/WebThemeEngine.h"

namespace BlinKit {

class WinThemeEngine final : public blink::WebThemeEngine
{
public:
    WinThemeEngine(void);
private:
    // blink::WebThemeEngine
    blink::WebSize getSize(Part part) override;
    void paint(blink::WebCanvas *canvas, Part part, State state, const blink::WebRect &rect, const ExtraParams *extra) override;

    typedef int(WINAPI * GetMetricsType)(int, UINT);
    GetMetricsType m_getMetrics;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
