// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: EncodedFormData.cpp
// Description: EncodedFormData Class
//      Author: Ziming Li
//     Created: 2021-07-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2006, 2008, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2012 Digia Plc. and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "platform/network/EncodedFormData.h"

#include "platform/FileMetadata.h"
#include "platform/network/FormDataEncoder.h"
#include "wtf/text/CString.h"
#include "wtf/text/TextEncoding.h"

namespace blink {

bool FormDataElement::isSafeToSendToAnotherThread() const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return m_filename.isSafeToSendToAnotherThread()
        && m_blobUUID.isSafeToSendToAnotherThread()
        && m_fileSystemURL.isSafeToSendToAnotherThread();
#endif
}

inline EncodedFormData::EncodedFormData()
    : m_identifier(0)
    , m_containsPasswordData(false)
{
}

inline EncodedFormData::EncodedFormData(const EncodedFormData& data)
    : RefCounted<EncodedFormData>()
    , m_elements(data.m_elements)
    , m_identifier(data.m_identifier)
    , m_containsPasswordData(data.m_containsPasswordData)
{
}

EncodedFormData::~EncodedFormData()
{
}

PassRefPtr<EncodedFormData> EncodedFormData::create()
{
    return adoptRef(new EncodedFormData);
}

PassRefPtr<EncodedFormData> EncodedFormData::create(const void* data, size_t size)
{
    RefPtr<EncodedFormData> result = create();
    result->appendData(data, size);
    return result.release();
}

PassRefPtr<EncodedFormData> EncodedFormData::create(const CString& string)
{
    RefPtr<EncodedFormData> result = create();
    result->appendData(string.data(), string.length());
    return result.release();
}

PassRefPtr<EncodedFormData> EncodedFormData::create(const Vector<char>& vector)
{
    RefPtr<EncodedFormData> result = create();
    result->appendData(vector.data(), vector.size());
    return result.release();
}

PassRefPtr<EncodedFormData> EncodedFormData::copy() const
{
    return adoptRef(new EncodedFormData(*this));
}

PassRefPtr<EncodedFormData> EncodedFormData::deepCopy() const
{
    RefPtr<EncodedFormData> formData(create());

    formData->m_identifier = m_identifier;
    formData->m_boundary = m_boundary;
    formData->m_containsPasswordData = m_containsPasswordData;

    size_t n = m_elements.size();
    formData->m_elements.reserveInitialCapacity(n);
    for (size_t i = 0; i < n; ++i) {
        const FormDataElement& e = m_elements[i];
        switch (e.m_type) {
        case FormDataElement::data:
            formData->m_elements.uncheckedAppend(FormDataElement(e.m_data));
            break;
        case FormDataElement::encodedFile:
            formData->m_elements.uncheckedAppend(FormDataElement(e.m_filename.isolatedCopy(), e.m_fileStart, e.m_fileLength, e.m_expectedFileModificationTime));
            break;
        case FormDataElement::encodedBlob:
            formData->m_elements.uncheckedAppend(FormDataElement(e.m_blobUUID.isolatedCopy(), e.m_optionalBlobDataHandle));
            break;
        case FormDataElement::encodedFileSystemURL:
            ASSERT(false); // BKTODO: formData->m_elements.uncheckedAppend(FormDataElement(e.m_fileSystemURL.copy(), e.m_fileStart, e.m_fileLength, e.m_expectedFileModificationTime));
            break;
        }
    }
    return formData.release();
}

void EncodedFormData::appendData(const void* data, size_t size)
{
    if (m_elements.isEmpty() || m_elements.last().m_type != FormDataElement::data)
        m_elements.append(FormDataElement());
    FormDataElement& e = m_elements.last();
    size_t oldSize = e.m_data.size();
    e.m_data.grow(oldSize + size);
    memcpy(e.m_data.data() + oldSize, data, size);
}

void EncodedFormData::appendFile(const String& filename)
{
    m_elements.append(FormDataElement(filename, 0, BlobDataItem::toEndOfFile, invalidFileTime()));
}

void EncodedFormData::appendFileRange(const String& filename, long long start, long long length, double expectedModificationTime)
{
    m_elements.append(FormDataElement(filename, start, length, expectedModificationTime));
}

void EncodedFormData::appendBlob(const String& uuid, PassRefPtr<BlobDataHandle> optionalHandle)
{
    m_elements.append(FormDataElement(uuid, optionalHandle));
}

void EncodedFormData::appendFileSystemURL(const KURL& url)
{
    m_elements.append(FormDataElement(url, 0, BlobDataItem::toEndOfFile, invalidFileTime()));
}

void EncodedFormData::appendFileSystemURLRange(const KURL& url, long long start, long long length, double expectedModificationTime)
{
    m_elements.append(FormDataElement(url, start, length, expectedModificationTime));
}

void EncodedFormData::flatten(Vector<char>& data) const
{
    // Concatenate all the byte arrays, but omit any files.
    data.clear();
    size_t n = m_elements.size();
    for (size_t i = 0; i < n; ++i) {
        const FormDataElement& e = m_elements[i];
        if (e.m_type == FormDataElement::data)
            data.append(e.m_data.data(), static_cast<size_t>(e.m_data.size()));
    }
}

String EncodedFormData::flattenToString() const
{
    Vector<char> bytes;
    flatten(bytes);
    return Latin1Encoding().decode(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

unsigned long long EncodedFormData::sizeInBytes() const
{
    unsigned size = 0;
    size_t n = m_elements.size();
    for (size_t i = 0; i < n; ++i) {
        const FormDataElement& e = m_elements[i];
        switch (e.m_type) {
        case FormDataElement::data:
            size += e.m_data.size();
            break;
        case FormDataElement::encodedFile:
            size += e.m_fileLength - e.m_fileStart;
            break;
        case FormDataElement::encodedBlob:
            if (e.m_optionalBlobDataHandle)
                size += e.m_optionalBlobDataHandle->size();
            break;
        case FormDataElement::encodedFileSystemURL:
            size += e.m_fileLength - e.m_fileStart;
            break;
        }
    }
    return size;
}

bool EncodedFormData::isSafeToSendToAnotherThread() const
{
    if (!hasOneRef())
        return false;
    for (auto& element : m_elements) {
        if (!element.isSafeToSendToAnotherThread())
            return false;
    }
    return true;
}

} // namespace blink
