// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: sign_request.h
// Description: SignRequest Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_SIGN_REQUEST_H
#define BLINKIT_BKLOGIN_SIGN_REQUEST_H

#pragma once

#include <openssl/x509.h>

namespace BlinKit {

class SignRequest
{
public:
    SignRequest(void);
    ~SignRequest(void);

    operator X509_REQ*() const
    {
#ifndef NDEBUG
        ASSERT(m_signed);
#endif
        return m_req;
    }

    void SetCommonName(const char *commonName);

    int Sign(EVP_PKEY *key);
private:
    X509_REQ *m_req;
#ifndef NDEBUG
    bool m_signed = false;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_SIGN_REQUEST_H
