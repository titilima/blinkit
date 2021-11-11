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

BK_DECLARE_HANDLE(BkWebView, WebViewImpl);

struct BkWebViewClient {
    size_t SizeOfStruct; // sizeof(BkWebViewClient)
    void *UserData;
    void (BKAPI * DocumentReady)(void *);
    bool_t (BKAPI * TitleChange)(const char *, void *);
};

BKEXPORT void BKAPI BkWebViewSetClient(BkWebView view, struct BkWebViewClient *client);

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI);

struct BkPoint { int x, y; };
struct BkSize { int width, height; };
struct BkRect { BkPoint location; BkSize size; };

BKEXPORT bool BKAPI BkQueryElementPosition(BkWebView view, const char *id, struct BkRect *dst);

#ifdef _WIN32

BKEXPORT BkWebView BKAPI BkGetWebView(HWND hWnd);

BKEXPORT LRESULT CALLBACK BkDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

BKEXPORT bool_t BKAPI BkProcessWindowMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result);

#endif // _WIN32

#ifdef __cplusplus
}
#endif

#endif // BLINKIT_SDK_UI_H
