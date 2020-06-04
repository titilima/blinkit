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
#include "blinkit/login/ssl/rsa_key.h"
#include "blinkit/login/ssl/sign_request.h"

namespace BlinKit {

SSLPair::SSLPair(const std::string &pemKey, const std::string &pemCert)
    : m_key(std::make_unique<PrivateKeyImpl>(pemKey)), m_cert(pemCert)
{
}

SSLPair::SSLPair(const std::string &domain, const SSLPair &caData) : m_key(std::make_unique<RSAKey>())
{
    SignRequest req;
    req.SetCommonName(domain.c_str());
    if (BK_ERR_SUCCESS == req.Sign(*m_key))
        m_cert.Sign(req, *caData.m_key, caData.m_cert, 3650);
}

void SSLPair::AttachTo(SSL_CTX *ctx)
{
    ASSERT(IsValid());
    SSL_CTX_use_certificate(ctx, m_cert);
    SSL_CTX_use_PrivateKey(ctx, *m_key);
}

bool SSLPair::IsValid(void) const
{
    EVP_PKEY *k = *m_key;
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

} // namespace BlinKit
