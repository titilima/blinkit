// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: BlinKit.h
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_BLINKIT_H
#define BLINKIT_SDK_BLINKIT_H

#pragma once

#ifdef _WIN32
#   define BKAPI    __stdcall
#   ifndef BLINKIT_EXPORTS
#       define BKEXPORT __declspec(dllimport)
#   endif
#endif

#ifndef BKAPI
#   define BKAPI
#endif

#ifndef BKEXPORT
#   define BKEXPORT
#endif

namespace BlinKit {

class BkCrawler;
class BkCrawlerClient;
class BkView;
class BkViewClient;

/**
 * Base Definitions
 */

class BkError {
public:
    enum {
        Success = 0,
        UnknownError,
    };
};

/**
 * Application
 */

class BkAppClient {
};

class BkApp {
public:
    virtual void BKAPI Exit(void) = 0;

    virtual BkCrawler* BKAPI CreateCrawler(BkCrawlerClient &client) = 0;
    virtual BkView* BKAPI CreateView(BkViewClient &client) = 0;
};

/**
 * View
 */

class BkViewClient {
};

class BkView {
public:
    virtual void BKAPI Destroy(void) = 0;
    virtual int BKAPI Load(const char *URI) = 0;
};

#ifdef _ATL_VER

template <class T, class Client = BkViewClient>
class BkViewImplT : public Client
{
protected:
    BkViewImplT(void) : m_view(::BkGetApp()->CreateView(*this)) {}

    BkView* GetView(void) { return m_view; }
    const BkView* GetView(void) const { return m_view; }
private:
    BkView *m_view;
};

#endif // _ATL_VER

} // namespace BlinKit

extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkInitialize(BlinKit::BkAppClient *client);
extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkGetApp(void);

#endif // BLINKIT_SDK_BLINKIT_H
