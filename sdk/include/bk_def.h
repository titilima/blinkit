// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_def.h
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_DEF_H
#define BLINKIT_SDK_DEF_H

#pragma once

#ifdef __cplusplus
#   include <cstddef>
#   include <cstring>
#   include <string>
#   include <vector>
#else
#   include <stddef.h>
#   include <string.h>
#endif

#ifdef _WIN32
#   define BKAPI    __stdcall
#   ifndef BLINKIT_EXPORTS
#       define BKEXPORT __declspec(dllimport)
#   endif
#endif

#ifndef BKEXPORT
#   define BKEXPORT
#endif

#ifdef __cplusplus
#define BK_DECLARE_HANDLE(name, impl)   class impl; typedef impl *name
#else
#define BK_DECLARE_HANDLE(name, impl)   struct impl; typedef struct impl *name
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct BkBuffer {
    void* (BKAPI * Allocator)(size_t, void *);
    void *UserData;
};

BKEXPORT void BKAPI BkSetBufferData(struct BkBuffer *buffer, const void *data, size_t size);

#define BkSetBufferString(buffer, s)    BkSetBufferData(buffer, (s), strlen(s));

struct BkSimpleBuffer {
    struct BkBuffer Buffer;
    union {
        const char *String;
        void *Blob;
    };
    size_t Size;
};

BKEXPORT struct BkBuffer* BKAPI BkInitializeSimpleBuffer(struct BkSimpleBuffer *buffer);
BKEXPORT void BKAPI BkFinalizeSimpleBuffer(struct BkSimpleBuffer *buffer);

#ifdef __cplusplus
} // extern "C"

namespace BlinKit {

template <typename Container>
class BkBufferImpl {
public:
    BkBufferImpl(Container &data)
    {
        m_buffer.Allocator = Alloc;
        m_buffer.UserData = &data;
    }
    operator BkBuffer* (void) const {
        BkBuffer *buf = const_cast<BkBuffer *>(&m_buffer);
        return buf;
    }
private:
    static void* BKAPI Alloc(size_t size, void *This) {
        Container *c = reinterpret_cast<Container *>(This);
        c->resize(size / sizeof(typename Container::value_type));
        return const_cast<typename Container::value_type *>(c->data());
    }
    BkBuffer m_buffer;
};

template <typename CharType>
inline BkBufferImpl<std::basic_string<CharType>> BkMakeBuffer(std::basic_string<CharType> &s)
{
    return BkBufferImpl<std::basic_string<CharType>>(s);
}

inline BkBufferImpl<std::vector<unsigned char>> BkMakeBuffer(std::vector<unsigned char> &v)
{
    return BkBufferImpl<std::vector<unsigned char>>(v);
}

} // namespace BlinKit

#endif // __cplusplus

#endif // BLINKIT_SDK_DEF_H
