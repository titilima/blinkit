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

#include <cassert>
#ifndef BLINKIT_DISABLE_FUNCTIONAL
#   include <functional>
#endif
#include <string>
#include <vector>

#ifdef _WIN32
#   include <Windows.h>
#   define BKAPI    __stdcall
#   ifndef BLINKIT_EXPORTS
#       define BKEXPORT __declspec(dllimport)
#   endif
#endif

#ifdef __APPLE__
#   include <TargetConditionals.h>
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
class BkRequest;
class BkRequestClient;
class BkRequestController;
class BkRetainedResponse;
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

class BkBuffer {
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

class BkValue {
public:
    enum Type {
        UnknownType,
        ErrorType,
        UndefinedType,
        NullType,
        BooleanType,
        NumberType,
        StringType,
        ObjectType,
        ArrayType
    };
    virtual Type BKAPI GetType(void) const = 0;

    virtual int BKAPI GetAsErrorCode(void) const = 0;
    virtual bool BKAPI GetAsBoolean(void) const = 0;
    virtual int BKAPI GetAsString(BkBuffer &dst) const = 0;
    virtual int BKAPI GetAsJSON(BkBuffer &dst) const = 0;
};

class BkArgList {
public:
    virtual int BKAPI PushInt(int arg) = 0;
    virtual int BKAPI PushString(const char *arg, size_t length = 0) = 0;
};

class BkFunctionContext {
public:
    virtual const char* BKAPI Name(void) const = 0;

    virtual size_t BKAPI ArgCount(void) const = 0;
    virtual const BkValue* BKAPI ArgAt(size_t i) const = 0;

    virtual int BKAPI ReturnAsBoolean(bool b) = 0;
    virtual int BKAPI ReturnAsString(const char *s, size_t length = 0) = 0;
    virtual int BKAPI ReturnAsJSON(const char *json, size_t length = 0) = 0;
};

class BkCallback {
public:
    virtual void BKAPI OnFunctionCall(BkFunctionContext &context) {
        assert(false); // Not implemented!
    }
    virtual void BKAPI OnPushArgs(BkArgList &argList) {
        // Optional. Leave it emtpy if no args to pass.
    }
    virtual void BKAPI OnReturn(const BkValue &retVal) {
        assert(false); // Not implemented!
    }
};

#ifndef BLINKIT_DISABLE_FUNCTIONAL

typedef std::function<void(const BkValue &)> BkLambda;

class BkLambdaCallback final : public BkCallback {
public:
    BkLambdaCallback(const BkLambda &callback) : m_callback(callback) {}
private:
    void BKAPI OnReturn(const BkValue &retVal) override {
        m_callback(retVal);
    }
    const BkLambda &m_callback;
};

#endif // BLINKIT_DISABLE_FUNCTIONAL

class BkResponse {
public:
    virtual int BKAPI GetCurrentURL(BkBuffer &URL) const = 0;
    virtual int BKAPI StatusCode(void) const = 0;

    virtual int BKAPI GetHeader(const char *name, BkBuffer &value) const = 0;

    virtual unsigned BKAPI CookiesCount(void) const = 0;
    virtual int BKAPI GetCookie(unsigned i, BkBuffer &cookie) const = 0;

    virtual int BKAPI GetBody(BkBuffer &body) const = 0;

    // -----------------------------------------------------------------------------------------------------------------
    // Optional Methods
    // BlinKit internal will not call these methods, so external HTTP service providers need not to implement them.
    virtual BkRetainedResponse* BKAPI Retain(void) const { return nullptr; }
    enum class Information {
        OriginalURL,
        HTTPVersion,
        ReasonPhrase,
    };
    virtual int GetInformation(Information i, BkBuffer &value) const {
        assert(false); // Not implemented!
        return BkError::NotFound;
    }
    // Optional Methods End
    // -----------------------------------------------------------------------------------------------------------------
};

class BkCrawlerResponse : public BkResponse {
public:
    virtual void BKAPI SetBody(const char *body, size_t length) = 0;

    virtual void BKAPI Continue(void) = 0;
    virtual void BKAPI Cancel(void) = 0;
private:
    BkRetainedResponse* BKAPI Retain(void) const override final {
        assert(false); // Not reached!
        return nullptr;
    }
};

/**
 * Application
 */

class BkAppClient {
public:
    virtual BkRequest* BKAPI CreateRequest(const char *URL, BkRequestClient &client) {
        // Return nullptr to apply the default request creator (BkCreateRequest).
        return nullptr;
    }
};

class BkApp {
public:
    virtual void BKAPI Exit(void) = 0;

    // Crawler Configurations
    // - `userAgent`: String
    // - `incantation`: String
    // - `scriptEnabled`: Boolean
    virtual BkCrawler* BKAPI CreateCrawler(BkCrawlerClient &client, BkBuffer *errorMessage = nullptr) = 0;

    virtual BkView* BKAPI CreateView(BkViewClient &client) = 0;
};

/**
 * Crawler
 */

class BkCrawlerClient {
public:
    // Example Script:
    // {
    //     userAgent: <User agent for crawler, optional>,
    //     incantation: <Eval incantation for access the crawler object, optional>,
    //     scriptEnabled: <Script enabled, optional>,
    // }
    virtual void BKAPI GetUserScript(BkBuffer &userScript) = 0;

    // In JS, call `crawler.notify(someData)` to pass data to BkCrawlerClient.
    virtual void BKAPI DataNotify(BkCrawler *crawler, const BkValue &data) {
        assert(false); // Not implemented!
    }
    virtual bool BKAPI Log(const char *s) {
        // `crawler.log(s)` Handler.
        // If client processes the log, return true.
        // Otherwise, BlinKit will print the log natively (OutputDebugString/NSLog/...).
        return false;
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Callbacks
    virtual void BKAPI ContextReady(BkCrawler *crawler) {}
    virtual void BKAPI RequestComplete(BkCrawler *crawler, BkCrawlerResponse *response) {
        response->Continue();
    }
    virtual void BKAPI RequestFailed(int errorCode) {
        assert(BkError::Success == errorCode);
    }
    virtual void BKAPI DocumentReady(BkCrawler *crawler) {}
    // -----------------------------------------------------------------------------------------------------------------
    // Cookies
    // BlinKit holds a cookiejar internally, and the caller could also manage cookies by GetCookie/SetCookie.
    virtual void BKAPI GetCookie(const char *URL, BkBuffer &cookie) {
        // GetCookie will be called before sending requests.
        // If the client provides cookies, cookiejar will not be used.
        // Example:
        //     cookie.Assign("foo1=bar1; foo2=bar2; foo3=bar3");
    }
    virtual bool BKAPI SetCookie(const char *cookie) {
        // SetCookie will be called after receiving responses.
        // If returns true, the current cookie will not be saved into cookiejar.
        return false;
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Other Optional Callbacks
    virtual void BKAPI GetUserAgent(BkBuffer &userAgent) {}
};

class BkCrawler {
public:
    virtual void BKAPI Destroy(void) = 0;
    virtual int BKAPI Load(const char *URL) = 0;

    virtual int BKAPI Eval(const char *code, size_t length, BkCallback *callback) = 0;

    virtual int BKAPI CallFunction(const char *name, BkCallback *callback = nullptr) = 0;
    virtual int BKAPI CallCrawler(const char *method, BkCallback *callback = nullptr) = 0;
    virtual int BKAPI RegisterCrawlerFunction(const char *name, BkCallback &functionImpl) = 0;

    virtual int BKAPI AccessCrawlerMember(const char *name, BkCallback &callback) = 0;

    inline int AddCode(const char *code, size_t length = 0) {
        return Eval(code, length, nullptr);
    }
#ifndef BLINKIT_DISABLE_FUNCTIONAL
    inline int Eval(const char *expr, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return Eval(expr, 0, &cb);
    }
    inline int CallFunction(const char *name, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return CallFunction(name, &cb);
    }
    inline int CallCrawler(const char *method, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return CallCrawler(method, &cb);
    }
    inline int AccessCrawlerMember(const char *name, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return AccessCrawlerMember(name, cb);
    }
#endif
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

#ifdef _WIN32
    virtual UINT_PTR AnimatorTimerId(void) const {
        // Override this method if you uses 1000 for your own timer.
        return 1000;
    }
#endif
};

class BkView {
public:
#if defined(_WIN32)
    typedef HWND    NativeView;
    typedef HDC     NativeCanvas;
#else
    typedef void    *NativeView, *NativeCanvas;
#endif

    virtual void BKAPI Destroy(void) = 0;
    virtual int BKAPI Load(const char *URI) = 0;
    virtual NativeView BKAPI GetNativeView(void) const = 0;

    virtual int BKAPI Eval(const char *code, size_t length, BkCallback *callback) = 0;
    virtual int BKAPI CallFunction(const char *name, BkCallback *callback = nullptr) = 0;
    virtual int BKAPI RegisterExternalFunction(const char *name, BkCallback &functionImpl) = 0;

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

    inline int AddCode(const char *code, size_t length = 0) {
        return Eval(code, length, nullptr);
    }
#ifndef BLINKIT_DISABLE_FUNCTIONAL
    inline int Eval(const char *expr, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return Eval(expr, 0, &cb);
    }
    inline int CallFunction(const char *name, const BkLambda &callback) {
        BkLambdaCallback cb(callback);
        return CallFunction(name, &cb);
    }
#endif
};

/**
 * HTTP
 */

class BkRequest {
public:
    // Generally, a request will be automatically destroyed after completed,
    // and the caller SHOULD NOT do anything after `Perform` called.
    // See also `RequireLifecycleController` for "cancelable" controllers.
    virtual int BKAPI Perform(void) = 0;

    virtual void BKAPI SetMethod(const char *method) = 0; // Default is "GET"
    virtual void BKAPI SetHeader(const char *name, const char *value) = 0;
    virtual void BKAPI SetBody(const void *data, size_t dataLength) = 0;

    // -----------------------------------------------------------------------------------------------------------------
    // Optional Methods
    // BlinKit internal will not call these methods, so external HTTP service providers need not to implement them.
    virtual void BKAPI SetTimeout(unsigned timeout /* in seconds */) {
        assert(false); // Not implemented!
    }
    virtual BkRequestController* BKAPI RequireLifecycleController(void) {
        // If this returns a valid controller, the request wll not be destroyed after completed.
        return nullptr;
    }
    // Optional Methods End
    // -----------------------------------------------------------------------------------------------------------------
    // Inline Helpers
    inline int Post(const char *charSet = "utf-8") {
        SetMethod("POST");

        std::string contentType("application/x-www-form-urlencoded;charset=");
        contentType += charSet;
        SetHeader("Content-Type", contentType.c_str());

        return Perform();
    }
    // Inline Helpers End
    // -----------------------------------------------------------------------------------------------------------------
};

class BkRequestController {
public:
    virtual void BKAPI Cancel(void) = 0;
    virtual void BKAPI Release(void) = 0;
};

class BkRetainedResponse : public BkResponse {
public:
    virtual void BKAPI Release(void) = 0;
};

class BkRequestClient
{
public:
    virtual void BKAPI RequestComplete(const BkResponse &response) = 0;
    virtual void BKAPI RequestFailed(int errorCode) {
        assert(BkError::Success == errorCode);
    }

    virtual bool BKAPI UseProxy(void) { return true; }
    virtual void BKAPI GetProxy(BkBuffer &proxy) {
        // Empty string proxy is for system global proxy.
    }
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

// Create a HTTP request. `BkInitialize` is not required.
extern "C" BKEXPORT BlinKit::BkRequest* BKAPI BkCreateRequest(const char *URL, BlinKit::BkRequestClient &client);

#endif // BLINKIT_SDK_BLINKIT_H
