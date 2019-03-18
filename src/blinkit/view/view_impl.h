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

#include <SkCanvas.h>
#include <SkColor.h>
#include "public/web/WebInputEvent.h"
#include "browser/browser_impl.h"

namespace BlinKit {

class ViewImpl : public BkView, public BrowserImpl
{
public:
    static ViewImpl* CreateInstance(BkViewClient &client);
    virtual ~ViewImpl(void);
protected:
    ViewImpl(BkViewClient &client);

    void FillCoordinates(blink::WebMouseEvent &dst, int x, int y);
    virtual std::unique_ptr<SkCanvas> CreateMemoryCanvas(int width, int height) = 0;
    virtual void DoUpdate(void);
    virtual blink::WebMouseEvent Translate(const MouseEvent &e);
    virtual void PreHandleInput(const blink::WebInputEvent &e);
    virtual void PostHandleInput(const blink::WebInputEvent &e);

    // BkView
    void BKAPI Destroy(void) override final { delete this; }
    void BKAPI ProcessInput(const MouseEvent &e) override final;
    void BKAPI Resize(int width, int height) override final;
    void BKAPI SetFocus(bool focused) override final;
    void BKAPI SetScaleFactor(float scaleFactor) override final;

    BkViewClient &m_client;
    std::unique_ptr<SkCanvas> m_memoryCanvas;
    bool m_hasDoubleClickEvent = false;
private:
    static SkColor BackgroundColor(void);
    blink::WebInputEvent::Type Translate(MouseEvent::Type t) const;
    static blink::WebMouseEvent::Button Translate(MouseEvent::Button b);
    bool UpdateRequired(void) const { return *m_updateRequired; }

    // BkView
    int BKAPI Load(const char *URI) override final;
    // blink::WebWidgetClient
    void scheduleAnimation(void) override final;
    // blink::WebFrameClient
    void didFinishLoad(blink::WebLocalFrame *frame) override final;

    std::shared_ptr<bool> m_updateRequired;

    //------------------------------------------------------------
    // Mouse Event Session
    static const double DoubleClickInterval;
    bool m_mouseEntered = false;
    int m_lastX = 0, m_lastY = 0;
    double m_lastDownTime = 0.0;
    int m_lastDownX = -1, m_lastDownY = -1;
    //------------------------------------------------------------
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_VIEW_IMPL_H
