// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: buffer.cpp
// Description: Buffer Related Implementations
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "bk_def.h"

#include "bkcommon/buffer_impl.hpp"

static void* BKAPI Alloc(size_t cb, void *userData)
{
    BkSimpleBuffer *buffer = reinterpret_cast<BkSimpleBuffer *>(userData);
    if (cb > 0)
    {
        buffer->Blob = malloc(cb);
        buffer->Size = cb;
    }
    return buffer->Blob;
}

extern "C" {

void BKAPI BkFinalizeSimpleBuffer(BkSimpleBuffer *buffer)
{
    buffer->Buffer.Allocator = nullptr;
    buffer->Buffer.UserData = nullptr;
    if (nullptr != buffer->Blob)
    {
        free(buffer->Blob);
        buffer->Blob = nullptr;
    }
    buffer->Size = 0;
}

BkBuffer* BKAPI BkInitializeSimpleBuffer(BkSimpleBuffer *buffer)
{
    buffer->Buffer.Allocator = Alloc;
    buffer->Buffer.UserData = buffer;
    buffer->Blob = nullptr;
    buffer->Size = 0;
    return &buffer->Buffer;
}

void BKAPI BkSetBufferData(BkBuffer *buffer, const void *data, size_t size)
{
    BlinKit::BufferImpl::Set(buffer, data, size);
}

} // extern "C"
