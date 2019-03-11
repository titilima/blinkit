// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_impl.h
// Description: ViewImpl Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_VIEW_IMPL_H
#define BLINKIT_BLINKIT_VIEW_IMPL_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "browser/browser_impl.h"

namespace BlinKit {

class ViewImpl : public BkView, public BrowserImpl
{
public:
    static ViewImpl* CreateInstance(BkViewClient &client);
    virtual ~ViewImpl(void);
protected:
    ViewImpl(BkViewClient &client);
    BkViewClient &m_client;
private:
    // BkView
    void BKAPI Destroy(void) override final { delete this; }
    int BKAPI Load(const char *URI) override final;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_VIEW_IMPL_H
