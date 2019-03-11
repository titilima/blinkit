// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_view.h
// Description: WinView Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_VIEW_H
#define BLINKIT_BLINKIT_WIN_VIEW_H

#pragma once

#include "view_impl.h"

namespace BlinKit {

class WinView final : public ViewImpl
{
public:
    WinView(BkViewClient &client);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_VIEW_H
