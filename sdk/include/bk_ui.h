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
    void (BKAPI * DocumentReady)(void *);
    bool_t (BKAPI * TitleChange)(const char *, void *);
};

BKEXPORT void BKAPI BkWebViewSetClient(BkWebView view, struct BkWebViewClient *client);

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI);

BKEXPORT BkElement BKAPI BkGetElementById(BkWebView view, const char *id);

#ifdef _WIN32

BKEXPORT BkWebView BKAPI BkGetWebView(HWND hWnd);

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

BKEXPORT bool_t BKAPI BkProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);

#endif // _WIN32

/**
 * Element
 */

BKEXPORT bool_t BKAPI BkGetElementAttribute(BkElement e, const char *name, struct BkBuffer *dst);

BKEXPORT void BKAPI BkSetElementAttribute(BkElement e, const char *name, const char *value);
BKEXPORT void BKAPI BkSetElementIntegalAttribute(BkElement e, const char *name, int value);

struct BkPoint { int x, y; };
struct BkSize { int width, height; };
struct BkRect { BkPoint location; BkSize size; };

BKEXPORT void BKAPI BkGetElementPosition(BkElement e, struct BkRect *dst);

#ifdef __cplusplus
}
#endif

#endif // BLINKIT_SDK_UI_H
