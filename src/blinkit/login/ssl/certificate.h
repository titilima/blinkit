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

    int Sign(X509_REQ *req, EVP_PKEY *key, int days = 365);

    int Save(const BkPathChar *fileName);
private:
    X509 *m_cert;
#ifndef NDEBUG
    bool m_signed = false;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_CERTIFICATE_H
