#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: SharedBuffer.h
// Description: SharedBuffer Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SHARED_BUFFER_H
#define BLINKIT_BLINKIT_SHARED_BUFFER_H

#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#   include "third_party/skia/include/core/SkData.h"
#endif

namespace blink {

class SharedBuffer : public std::enable_shared_from_this<SharedBuffer>
{
public:
    static std::shared_ptr<SharedBuffer> create(void);
    static std::shared_ptr<SharedBuffer> create(const char *data, size_t length);
    static std::shared_ptr<SharedBuffer> create(const unsigned char* data, size_t length);
    static std::shared_ptr<SharedBuffer> adoptVector(std::vector<char> &v);

    const char* data(void) const { return m_data.empty() ? nullptr : m_data.data(); }
    size_t size(void) const { return m_data.size(); }
    bool isEmpty(void) const { return m_data.empty(); }

    size_t getSomeData(const char *&data, size_t position) const;

    void append(const char *data, size_t length);
    void clear(void) { m_data.clear(); }

    bool isLocked(void) const;

#ifdef BLINKIT_UI_ENABLED
    // Creates an SkData and copies this SharedBuffer's contents to that
    // SkData without merging segmented buffers into a flat buffer.
    PassRefPtr<SkData> getAsSkData(void) const;
#endif
private:
    SharedBuffer(void) = default;
    SharedBuffer(const char *data, size_t length);

    std::vector<char> m_data;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_SHARED_BUFFER_H
