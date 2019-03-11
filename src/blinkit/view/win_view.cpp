// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_view.cpp
// Description: WinView Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_view.h"

namespace BlinKit {

WinView::WinView(BkViewClient &client) : ViewImpl(client)
{
    // Nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ViewImpl* ViewImpl::CreateInstance(BkViewClient &client)
{
    return new WinView(client);
}

} // namespace BlinKit
