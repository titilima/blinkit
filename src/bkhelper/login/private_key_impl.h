// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: private_key_impl.h
// Description: PrivateKeyImpl Class
//      Author: Ziming Li
//     Created: 2020-06-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_PRIVATE_KEY_IMPL_H
#define BLINKIT_BKHELPER_PRIVATE_KEY_IMPL_H

#pragma once

#include <openssl/ossl_typ.h>

class PrivateKeyImpl
{
public:
    PrivateKeyImpl(const std::string &pem);
    virtual ~PrivateKeyImpl(void);

    operator EVP_PKEY*() const { return m_key; }

    virtual int Save(const BkPathChar *fileName);
protected:
    PrivateKeyImpl(void) = default;

    EVP_PKEY *m_key = nullptr;
};

#endif // BLINKIT_BKHELPER_PRIVATE_KEY_IMPL_H
