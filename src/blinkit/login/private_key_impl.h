// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: private_key_impl.h
// Description: PrivateKeyImpl Class
//      Author: Ziming Li
//     Created: 2020-06-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_PRIVATE_KEY_IMPL_H
#define BLINKIT_BKLOGIN_PRIVATE_KEY_IMPL_H

#pragma once

#include <openssl/ossl_typ.h>

class PrivateKeyImpl
{
public:
    PrivateKeyImpl(int bits = 2048);
    ~PrivateKeyImpl(void);

    operator EVP_PKEY*() { return m_key; }

    int Save(const BkPathChar *fileName);
private:
    EVP_PKEY *m_key;
    RSA *m_rsa;
};

#endif // BLINKIT_BKLOGIN_PRIVATE_KEY_IMPL_H
