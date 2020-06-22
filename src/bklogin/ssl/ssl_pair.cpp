// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: ssl_pair.cpp
// Description: SSLPair Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "ssl_pair.h"

#include <openssl/ssl.h>
#include "bklogin/ssl/rsa_key.h"
#include "bklogin/ssl/sign_request.h"

namespace BlinKit {

SSLPair::SSLPair(const std::string &pemKey, const std::string &pemCert) : m_key(pemKey), m_cert(pemCert)
{
}

bool SSLPair::IsValid(void) const
{
    EVP_PKEY *k = m_key;
    if (nullptr == k)
    {
        ASSERT(nullptr != k);
        return false;
    }

    X509 *x = m_cert;
    if (nullptr == x)
    {
        ASSERT(nullptr != x);
        return false;
    }

    if (0 == X509_check_private_key(x, k))
    {
        ASSERT(false); // Check failed!
        return false;
    }
    return true;
}

SSL_CTX* SSLPair::NewDomainContext(const std::string &domain)
{
    RSAKey key;

    SignRequest req;
    req.SetCommonName(domain.c_str());
    if (BK_ERR_SUCCESS != req.Sign(key))
        return nullptr;

    Certificate cert;
    if (BK_ERR_SUCCESS != cert.Sign(req, m_key, m_cert, 3650))
        return nullptr;

    SSL_CTX *ctx = SSL_CTX_new(SSLv23_server_method());
    SSL_CTX_use_certificate(ctx, cert);
    SSL_CTX_use_PrivateKey(ctx, key);
    return ctx;
}

} // namespace BlinKit
