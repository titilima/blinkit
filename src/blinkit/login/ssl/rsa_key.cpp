// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: rsa_key.cpp
// Description: RSAKey Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "rsa_key.h"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include "blinkit/common/bk_file.h"

namespace BlinKit {

RSAKey::RSAKey(int bits) : m_rsa(RSA_new())
{
    m_key = EVP_PKEY_new();

    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);
    RSA_generate_multi_prime_key(m_rsa, bits, 2, bn, nullptr);
    BN_free(bn);

    EVP_PKEY_assign_RSA(m_key, m_rsa); // m_rsa is attached to m_key, so need not to free it.
}

int RSAKey::Save(const BkPathChar *fileName)
{
    BIO *mem = BIO_new(BIO_s_mem());
    if (nullptr == mem)
    {
        ASSERT(nullptr != mem);
        return BK_ERR_UNKNOWN;
    }

    int r = PEM_write_bio_RSAPrivateKey(mem, m_rsa, nullptr, nullptr, 0, nullptr, nullptr);
    if (0 != r)
    {
        char *data = nullptr;
        long l = BIO_get_mem_data(mem, &data);
        r = BkFile::WriteContent(fileName, data, l);
    }
    else
    {
        ASSERT(0 != r);
        r = BK_ERR_UNKNOWN;
    }

    BIO_free(mem);
    return r;
}

} // namespace BlinKit
