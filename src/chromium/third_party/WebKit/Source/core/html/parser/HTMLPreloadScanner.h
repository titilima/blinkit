// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: HTMLPreloadScanner.h
// Description: HTMLPreloadScanner Class
//      Author: Ziming Li
//     Created: 2019-05-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2010 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HTMLPreloadScanner_h
#define HTMLPreloadScanner_h

#include "core/CoreExport.h"
#include "core/css/MediaValues.h"
#include "core/dom/ViewportDescription.h"
#include "core/html/parser/CSSPreloadScanner.h"
#include "core/html/parser/CompactHTMLToken.h"
#include "core/html/parser/HTMLToken.h"
#include "platform/text/SegmentedString.h"
#include "wtf/RefCounted.h"
#include "wtf/Vector.h"

namespace blink {

typedef size_t TokenPreloadScannerCheckpoint;

class HTMLParserOptions;
class HTMLTokenizer;
class SegmentedString;

struct CORE_EXPORT CachedDocumentParameters {
    USING_FAST_MALLOC(CachedDocumentParameters);
public:
    static PassOwnPtr<CachedDocumentParameters> create(Document* document, PassRefPtrWillBeRawPtr<MediaValues> mediaValues = nullptr)
    {
        return adoptPtr(new CachedDocumentParameters(document, mediaValues));
    }

    bool doHtmlPreloadScanning;
#ifndef BLINKIT_CRAWLER_ONLY
    bool forCrawler;
    RefPtrWillBeCrossThreadPersistent<MediaValues> mediaValues;
    Length defaultViewportMinWidth;
#endif
    bool viewportMetaZeroValuesQuirk;
    bool viewportMetaEnabled;
    ReferrerPolicy referrerPolicy;

private:
    CachedDocumentParameters(Document*, PassRefPtrWillBeRawPtr<MediaValues>);
};

class TokenPreloadScanner {
    WTF_MAKE_NONCOPYABLE(TokenPreloadScanner); USING_FAST_MALLOC(TokenPreloadScanner);
public:
    TokenPreloadScanner(const KURL& documentURL, PassOwnPtr<CachedDocumentParameters>);
    ~TokenPreloadScanner();

    void scan(const HTMLToken&, const SegmentedString&, PreloadRequestStream& requests);
    void scan(const CompactHTMLToken&, const SegmentedString&, PreloadRequestStream& requests);

    void setPredictedBaseElementURL(const KURL& url) { m_predictedBaseElementURL = url; }

    // A TokenPreloadScannerCheckpoint is valid until the next call to rewindTo,
    // at which point all outstanding checkpoints are invalidated.
    TokenPreloadScannerCheckpoint createCheckpoint();
    void rewindTo(TokenPreloadScannerCheckpoint);

    bool isSafeToSendToAnotherThread()
    {
        return m_documentURL.isSafeToSendToAnotherThread()
            && m_predictedBaseElementURL.isSafeToSendToAnotherThread();
    }

private:
    class StartTagScanner;

    template <typename Token>
    inline void scanCommon(const Token&, const SegmentedString&, PreloadRequestStream& requests);

    template<typename Token>
    void updatePredictedBaseURL(const Token&);

    struct Checkpoint {
#ifdef BLINKIT_CRAWLER_ONLY
        Checkpoint(const KURL& predictedBaseElementURL)
#else
        Checkpoint(const KURL& predictedBaseElementURL, bool inStyle, size_t templateCount)
#endif
            : predictedBaseElementURL(predictedBaseElementURL)
#ifndef BLINKIT_CRAWLER_ONLY
            , inStyle(inStyle)
            , templateCount(templateCount)
#endif
        {
        }

        KURL predictedBaseElementURL;
#ifndef BLINKIT_CRAWLER_ONLY
        bool inStyle;
        size_t templateCount;
#endif
    };

    struct PictureData {
        PictureData()
            : sourceSize(0.0)
            , sourceSizeSet(false)
            , picked(false)
        {
        }
        String sourceURL;
        float sourceSize;
        bool sourceSizeSet;
        bool picked;
    };

#ifndef BLINKIT_CRAWLER_ONLY
    CSSPreloadScanner m_cssScanner;
#endif
    const KURL m_documentURL;
    KURL m_predictedBaseElementURL;
#ifndef BLINKIT_CRAWLER_ONLY
    bool m_inStyle = false;
    bool m_inPicture = false;
    PictureData m_pictureData;
    size_t m_templateCount = 0;
#endif
    OwnPtr<CachedDocumentParameters> m_documentParameters;

    Vector<Checkpoint> m_checkpoints;
};

class CORE_EXPORT HTMLPreloadScanner {
    WTF_MAKE_NONCOPYABLE(HTMLPreloadScanner); USING_FAST_MALLOC(HTMLPreloadScanner);
public:
    static PassOwnPtr<HTMLPreloadScanner> create(const HTMLParserOptions& options, const KURL& documentURL, PassOwnPtr<CachedDocumentParameters> documentParameters)
    {
        return adoptPtr(new HTMLPreloadScanner(options, documentURL, documentParameters));
    }


    HTMLPreloadScanner(const HTMLParserOptions&, const KURL& documentURL, PassOwnPtr<CachedDocumentParameters>);
    ~HTMLPreloadScanner();

    void appendToEnd(const SegmentedString&);
    void scan(ResourcePreloader*, const KURL& documentBaseElementURL);

private:
    TokenPreloadScanner m_scanner;
    SegmentedString m_source;
    HTMLToken m_token;
    OwnPtr<HTMLTokenizer> m_tokenizer;
};

}

#endif
