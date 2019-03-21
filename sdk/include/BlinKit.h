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
        InvalidOption,
        Forbidden,
        NotFound,
        NetworkError,
        Cancelled,
        Exception,
        EvalError,
        RangeError,
        ReferenceError,
        SyntaxError,
        TypeError,
        URIError
    };
};

class BkBuffer
{
public:
    inline void Assign(const std::string &s) {
        void *dst = Prepare(s.length());
        memcpy(dst, s.data(), s.length());
    }
    inline void Assign(const void *data, size_t size) {
        void *dst = Prepare(size);
        memcpy(dst, data, size);
    }
protected:
    virtual void* BKAPI Prepare(size_t cb) = 0;
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
 * Crawler
 */

class BkCrawlerClient {
public:
    virtual void BKAPI DocumentReady(BkCrawler *crawler) {}
};

class BkCrawler {
public:
    virtual void BKAPI Destroy(void) = 0;
    virtual int BKAPI Load(const char *URI) = 0;

    virtual void BKAPI SetUserAgent(const char *userAgent) = 0;
};

/**
 * View
 */

struct BkSize {
    int width, height;
};

struct BkRect {
    int x, y, width, height;
};

class BkViewClient {
public:
    virtual void BKAPI DocumentReady(BkView *view) {}

    virtual bool BKAPI AllowTitleChange(BkView *view, const char *newTitle) {
        // Return true to allow BlinKit change the title of the native view.
        return true;
    }
};

class BkView {
public:
#if defined(_WIN32)
    typedef HWND    NativeView;
    typedef HDC     NativeCanvas;
#else
#endif

    virtual void BKAPI Destroy(void) = 0;
    virtual int BKAPI Load(const char *URI) = 0;
    virtual NativeView BKAPI GetNativeView(void) const = 0;
#ifdef _WIN32
    /**
     * If ProcessMessage returns false, it means default message handlers (e.g. DefWindowProc) needed.
     */
    virtual bool BKAPI ProcessMessage(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT &r) = 0;
#endif

    struct MouseEvent {
        enum Type { MouseDown, MouseUp, MouseMove, MouseLeave, ContextMenu };
        enum Button { NoButton, LeftButton, MiddleButton, RightButton };

        Type type;
        Button button;
        int x, y;
    };
    virtual void BKAPI ProcessInput(const MouseEvent &e) = 0;

    struct KeyboardEvent {
        enum Type { KeyDown, KeyUp, Char };

        Type type;
        int code;
        bool shiftPressed;
        bool ctrlPressed;
        bool altPressed;
        bool fromKeyPad;

        KeyboardEvent(void) : shiftPressed(false), ctrlPressed(false), altPressed(false), fromKeyPad(false) {}
    };
    virtual void BKAPI ProcessInput(const KeyboardEvent &e) = 0;

    virtual void BKAPI Attach(NativeView nativeView) = 0;
    virtual void BKAPI Paint(NativeCanvas nativeCanvas, const BkRect *rc = nullptr) = 0;
    virtual void BKAPI Resize(int width, int height) = 0;
    virtual void BKAPI SetFocus(bool focused) = 0;
    virtual bool BKAPI GetCaretRect(BkRect *dst) = 0;
    virtual void BKAPI SetScaleFactor(float scaleFactor) = 0;
};

/**
 * Helpers
 */

class BkBufferImpl : public BkBuffer
{
public:
    BkBuffer& Wrap(void) const {
        BkBuffer &o = const_cast<BkBufferImpl &>(*this);
        return o;
    }
protected:
    BkBufferImpl(void) = default;
};

template <typename CharType>
class BkStringBuffer final : public BkBufferImpl
{
public:
    BkStringBuffer(std::basic_string<CharType> &s) : m_s(s) {}
private:
    void* BKAPI Prepare(size_t cb) override {
        m_s.resize(cb / sizeof(CharType));
        return const_cast<CharType *>(m_s.data());
    }
    std::basic_string<CharType> &m_s;
};

template <typename CharType>
inline BkStringBuffer<CharType> BkMakeBuffer(std::basic_string<CharType> &s)
{
    return BkStringBuffer<CharType>(s);
}

class BkBlobBuffer final : public BkBufferImpl
{
public:
    BkBlobBuffer(std::vector<unsigned char> &v) : m_v(v) {}
private:
    void* BKAPI Prepare(size_t cb) override {
        m_v.resize(cb);
        return m_v.data();
    }
    std::vector<unsigned char> &m_v;
};

inline BkBlobBuffer BkMakeBuffer(std::vector<unsigned char> &v)
{
    return BkBlobBuffer(v);
}

#ifdef _ATL_VER

template <class T, class Client = BkViewClient>
class BkViewImplT : public Client
{
protected:
    BkViewImplT(void) : m_view(::BkGetApp()->CreateView(*this)) {}

    BkView* GetView(void) { return m_view; }
    const BkView* GetView(void) const { return m_view; }

    bool ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)
    {
        return m_view->ProcessMessage(hWnd, uMsg, wParam, lParam, lResult);
    }

private:
    BkView *m_view;
};

#endif // _ATL_VER

} // namespace BlinKit

extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkInitialize(BlinKit::BkAppClient *client);
extern "C" BKEXPORT BlinKit::BkApp* BKAPI BkGetApp(void);

#endif // BLINKIT_SDK_BLINKIT_H
