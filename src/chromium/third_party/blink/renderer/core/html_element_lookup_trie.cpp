// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_lookup_trie.cpp
// Description: lookupHTMLTag
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "html_element_lookup_trie.h"

#include "third_party/blink/renderer/core/html_names.h"

namespace blink {

using namespace html_names;

static const HTMLQualifiedName* LookupHTMLTag1(const UChar *data)
{
    switch (*data)
    {
        case 'a': return &kATag;
        case 'p': return &kPTag;
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag2(const UChar *data)
{
    switch (data[0])
    {
        case 'h':
        {
            switch (data[1])
            {
                case '1': return &kH1Tag;
                case '2': return &kH2Tag;
                case '3': return &kH3Tag;
                case '4': return &kH4Tag;
                case '5': return &kH5Tag;
                case '6': return &kH6Tag;
            }
            break;
        }
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag3(const UChar *data)
{
    switch (data[0])
    {
        case 'd':
        {
            if ('i' == data[1] && 'v' == data[2])
                return &kDivTag;
            break;
        }
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag4(const UChar *data)
{
    switch (data[0])
    {
        case 'b':
        {
            if ('o' == data[1] && 'd' == data[2] && 'y' == data[3])
                return &kBodyTag;
            break;
        }
        case 'h':
        {
            if ('e' == data[1] && 'a' == data[2] && 'd' == data[3])
                return &kHeadTag;
            if ('t' == data[1] && 'm' == data[2] && 'l' == data[3])
                return &kHTMLTag;
            break;
        }
        case 'm':
        {
            if ('e' == data[1] && 't' == data[2] && 'a' == data[3])
                return &kMetaTag;
            break;
        }
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag5(const UChar *data)
{
    switch (data[0])
    {
        case 's':
        {
            if ('t' == data[1] && 'y' == data[2] && 'l' == data[3] && 'e' == data[4])
                return &kStyleTag;
            break;
        }
        case 't':
        {
            if ('a' == data[1] && 'b' == data[2] && 'l' == data[3] && 'e' == data[4])
                return &kTableTag;
            if ('i' == data[1] && 't' == data[2] && 'l' == data[3] && 'e' == data[4])
                return &kTitleTag;
            break;
        }
    }
    return nullptr;
}

const AtomicString& lookupHTMLTag(const UChar *data, unsigned length)
{
    ASSERT(nullptr != data);
    ASSERT(0 != length);

    const HTMLQualifiedName *tag = nullptr;
    switch (length)
    {
        case 1:
            tag = LookupHTMLTag1(data);
            break;
        case 2:
            tag = LookupHTMLTag2(data);
            break;
        case 3:
            tag = LookupHTMLTag3(data);
            break;
        case 4:
            tag = LookupHTMLTag4(data);
            break;
        case 5:
            tag = LookupHTMLTag5(data);
            break;
    }

    if (nullptr != tag)
        return tag->LocalName();
    ASSERT(false); // BKTODO:
    return g_null_atom;
}

} // namespace blink
