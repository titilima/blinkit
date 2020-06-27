// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: ssl_pair.h
// Description: SSLPair Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_SSL_PAIR_H
#define BLINKIT_BKHELPER_SSL_PAIR_H

#pragma once

#include "bkhelper/ssl/private_key_impl.h"
#include "bkhelper/ssl/certificate.h"

namespace BlinKit {

class SSLPair
{
public:
    SSLPair(const std::string &pemKey, const std::string &pemCert);

    bool IsValid(void) const;

    SSL_CTX* NewDomainContext(const std::string &domain);
private:
    PrivateKeyImpl m_key;
    Certificate m_cert;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_SSL_PAIR_H
