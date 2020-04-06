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
        case 'i': return &kITag;
        case 'p': return &kPTag;
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag2(const UChar *data)
{
    switch (data[0])
    {
        case 'b':
        {
            if ('r' == data[1])
                return &kBrTag;
            break;
        }
        case 'e':
        {
            if ('m' == data[1])
                return &kEmTag;
            break;
        }
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
                case 'r': return &kHrTag;
            }
            break;
        }
        case 'l':
        {
            if ('i' == data[1])
                return &kLiTag;
            break;
        }
        case 't':
        {
            switch (data[1])
            {
                case 'd': return &kTdTag;
                case 'h': return &kThTag;
                case 'r': return &kTrTag;
            }
            break;
        }
        case 'u':
        {
            if ('l' == data[1])
                return &kUlTag;
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
        case 'i':
        {
            if ('m' == data[1] && 'g' == data[2])
                return &kImgTag;
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
        case 'c':
        {
            if ('o' == data[1] && 'd' == data[2] && 'e' == data[3])
                return &kCodeTag;
            break;
        }
        case 'f':
        {
            if ('o' == data[1] && 'r' == data[2] && 'm' == data[3])
                return &kFormTag;
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
        case 'l':
        {
            if ('i' == data[1] && 'n' == data[2] && 'k' == data[3])
                return &kLinkTag;
            break;
        }
        case 'm':
        {
            if ('e' == data[1] && 't' == data[2] && 'a' == data[3])
                return &kMetaTag;
            break;
        }
        case 's':
        {
            if ('p' == data[1] && 'a' == data[2] && 'n' == data[3])
                return &kSpanTag;
            break;
        }
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag5(const UChar *data)
{
    switch (data[0])
    {
        case 'i':
        {
            if ('n' == data[1] && 'p' == data[2] && 'u' == data[3] && 't' == data[4])
                return &kInputTag;
            break;
        }
        case 'l':
        {
            if ('a' == data[1] && 'b' == data[2] && 'e' == data[3] && 'l' == data[4])
                return &kLabelTag;
            break;
        }
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

static const HTMLQualifiedName* LookupHTMLTag6(const UChar *data)
{
    switch (data[0])
    {
        case 'b':
        {
            if ('u' == data[1] && 't' == data[2] && 't' == data[3] && 'o' == data[4] && 'n' == data[5])
                return &kButtonTag;
            break;
        }
        case 'o':
        {
            if ('p' == data[1] && 't' == data[2] && 'i' == data[3] && 'o' == data[4] && 'n' == data[5])
                return &kOptionTag;
            break;
        }
        case 's':
        {
            if ('c' == data[1] && 'r' == data[2] && 'i' == data[3] && 'p' == data[4] && 't' == data[5])
                return &kScriptTag;
            if ('e' == data[1] && 'l' == data[2] && 'e' == data[3] && 'c' == data[4] && 't' == data[5])
                return &kSelectTag;
            if ('t' == data[1] && 'r' == data[2] && 'o' == data[3] && 'n' == data[4] && 'g' == data[5])
                return &kStrongTag;
            break;
        }
    }
    return nullptr;
}

static const HTMLQualifiedName* LookupHTMLTag8(const UChar *data)
{
    switch (data[0])
    {
        case 'f':
        {
            if ('i' == data[1] && 'e' == data[2] && 'l' == data[3] && 'd' == data[4] && 's' == data[5] && 'e' == data[6] && 't' == data[7])
                return &kFieldsetTag;
            break;
        }
        case 'n':
        {
            if ('o' == data[1] && 's' == data[2] && 'c' == data[3] && 'r' == data[4] && 'i' == data[5] && 'p' == data[6] && 't' == data[7])
                return &kNoscriptTag;
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
        case 6:
            tag = LookupHTMLTag6(data);
            break;
        case 8:
            tag = LookupHTMLTag8(data);
            break;
    }

    if (nullptr != tag)
        return tag->LocalName();
    ASSERT(nullptr == tag); // BKTODO:
    return g_null_atom;
}

} // namespace blink
