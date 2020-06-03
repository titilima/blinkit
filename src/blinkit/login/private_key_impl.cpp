// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: private_key_impl.cpp
// Description: PrivateKeyImpl Class
//      Author: Ziming Li
//     Created: 2020-06-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "private_key_impl.h"

#include "bk_login.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include "blinkit/common/bk_file.h"
#include "blinkit/login/login_globals.h"
#include "blinkit/login/ssl/certificate.h"
#include "blinkit/login/ssl/sign_request.h"

using namespace BlinKit;

PrivateKeyImpl::PrivateKeyImpl(int bits) : m_key(EVP_PKEY_new()), m_rsa(RSA_new())
{
    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);
    RSA_generate_multi_prime_key(m_rsa, bits, 2, bn, nullptr);
    BN_free(bn);

    EVP_PKEY_assign_RSA(m_key, m_rsa);
}

PrivateKeyImpl::~PrivateKeyImpl(void)
{
    EVP_PKEY_free(m_key);
}

int PrivateKeyImpl::Save(const BkPathChar *fileName)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkPrivateKey BKAPI BkCreatePrivateKey(void)
{
    LoginGlobals::EnsureSSLInitialized();
    return new PrivateKeyImpl;
}

BKEXPORT void BKAPI BkDestroyPrivateKey(BkPrivateKey key)
{
    delete key;
}

BKEXPORT int BKAPI BkSavePrivateKey(BkPrivateKey key, const BkPathChar *fileName)
{
    return key->Save(fileName);
}

BKEXPORT int BKAPI BkSignPrivateKey(BkPrivateKey key, const char *commonName, int days, const BkPathChar *certFileName)
{
    SignRequest req;
    req.SetCommonName(commonName);
    int r = req.Sign(*key);
    if (BK_ERR_SUCCESS != r)
        return r;

    Certificate cert;
    r = cert.Sign(req, *key, days);
    if (BK_ERR_SUCCESS != r)
        return r;
    return cert.Save(certFileName);
}

} // extern "C"
