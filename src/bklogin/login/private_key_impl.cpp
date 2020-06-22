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
#include "bklogin/login/login_globals.h"
#include "bklogin/login/ssl/certificate.h"
#include "bklogin/login/ssl/rsa_key.h"
#include "bklogin/login/ssl/sign_request.h"

using namespace BlinKit;

PrivateKeyImpl::PrivateKeyImpl(const std::string &pem)
{
    BIO *bio = BIO_new_mem_buf(pem.data(), pem.length());
    m_key = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
}

PrivateKeyImpl::~PrivateKeyImpl(void)
{
    if (nullptr != m_key)
        EVP_PKEY_free(m_key);
}

int PrivateKeyImpl::Save(const BkPathChar *fileName)
{
    NOTREACHED();
    return BK_ERR_FORBIDDEN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkPrivateKey BKAPI BkCreatePrivateKey(void)
{
    LoginGlobals::EnsureSSLInitialized();
    return new RSAKey;
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
    r = cert.SelfSign(req, *key, days);
    if (BK_ERR_SUCCESS != r)
        return r;
    return cert.Save(certFileName);
}

} // extern "C"
