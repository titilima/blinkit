// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_ui.h
//      Author: Ziming Li
//     Created: 2020-10-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_UI_H
#define BLINKIT_SDK_UI_H

#pragma once

#include "bk_def.h"
#ifdef _WIN32
#   include <windef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Web View
 */

BK_DECLARE_HANDLE(BkWebView, WebViewImpl);
BK_DECLARE_HANDLE(BkElement, ElementImpl);

struct BkWebViewClient {
    size_t SizeOfStruct; // sizeof(BkWebViewClient)
    void *UserData;

    void (BKAPI * DocumentReady)(BkWebView, void *);

#ifdef _WIN32
    bool_t (BKAPI * ProcessMessage)(HWND, UINT, WPARAM, LPARAM, LRESULT *, void *);
#endif

    bool_t (BKAPI * TitleChange)(BkWebView, const char *, void *);
    void (BKAPI * SizeChanged)(BkWebView, int, int, void *);
    void (BKAPI * LoadURI)(BkWebView, const char *, void *);
};

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI);

BKEXPORT BkElement BKAPI BkGetElementById(BkWebView view, const char *id);

typedef void (BKAPI * BkClickObserver)(void *);

BKEXPORT bool_t BKAPI BkAddClickObserver(BkWebView view, const char *id, BkClickObserver ob, void *userData);

BKEXPORT BkJSContext BKAPI BkGetJSContextFromWebView(BkWebView view);

#ifdef _WIN32

#   ifndef BLINKIT_EXPORTS
inline HWND BkCreateWebViewWindow(
    PCTSTR lpClassName,
    PCTSTR lpWindowName,
    DWORD dwStyle,
    int x, int y, int nWidth, int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    struct BkWebViewClient *client)
{
    return CreateWindow(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, client);
}

inline HWND BkCreateWebViewControl(
    PCTSTR lpClassName,
    DWORD dwStyle,
    int x, int y, int nWidth, int nHeight,
    HWND hWndParent,
    UINT uID,
    HINSTANCE hInstance,
    struct BkWebViewClient *client)
{
    dwStyle |= WS_CHILD;
    return BkCreateWebViewWindow(lpClassName, NULL, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)uID, hInstance, client);
}
#   endif

BKEXPORT BkWebView BKAPI BkGetWebView(HWND hWnd);

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif // _WIN32

/**
 * Element
 */

BKEXPORT bool_t BKAPI BkGetElementAttribute(BkElement e, const char *name, struct BkBuffer *dst);

BKEXPORT void BKAPI BkSetElementAttribute(BkElement e, const char *name, const char *value);
BKEXPORT void BKAPI BkSetElementIntegalAttribute(BkElement e, const char *name, int value);

enum BkPropertyId {
    BK_PROP_TEXTFIELD_VALUE = 0,
};

BKEXPORT bool_t BKAPI BkGetElementProperty(BkElement e, int propId, struct BkBuffer *dst);
BKEXPORT bool_t BKAPI BkSetElementProperty(BkElement e, int propId, const char *value, bool notify);

BKEXPORT void BKAPI BkAddClassToElement(BkElement e, const char *className);
BKEXPORT void BKAPI BkRemoveClassFromElement(BkElement e, const char *className);
BKEXPORT void BKAPI BkToggleElementClass(BkElement e, const char *className);

struct BkPoint { int x, y; };
struct BkSize { int width, height; };
struct BkRect { BkPoint location; BkSize size; };

BKEXPORT void BKAPI BkGetElementPosition(BkElement e, struct BkRect *dst, unsigned reserved);

#ifdef __cplusplus
}
#endif

#endif // BLINKIT_SDK_UI_H
