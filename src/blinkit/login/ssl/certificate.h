// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: certificate.h
// Description: Certificate Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_CERTIFICATE_H
#define BLINKIT_BKLOGIN_CERTIFICATE_H

#pragma once

#include <openssl/x509.h>

namespace BlinKit {

class Certificate
{
public:
    Certificate(void);
    Certificate(const std::string &pem);
    ~Certificate(void);

    operator X509*() const { return m_cert; }

    int SelfSign(X509_REQ *req, EVP_PKEY *key, int days = 365);
    int Sign(X509_REQ *req, EVP_PKEY *caKey, X509 *caCert, int days = 365);

    int Save(const BkPathChar *fileName);
private:
    void FillBasicData(X509_REQ *req, int days);

    X509 *m_cert;
#ifndef NDEBUG
    bool m_signed = false;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_CERTIFICATE_H
