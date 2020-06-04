// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: rsa_key.h
// Description: RSAKey Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_RSA_KEY_H
#define BLINKIT_BKLOGIN_RSA_KEY_H

#pragma once

#include "blinkit/login/private_key_impl.h"

namespace BlinKit {

class RSAKey final : public PrivateKeyImpl
{
public:
    RSAKey(int bits = 2048);

    int Save(const BkPathChar *fileName) override;
private:
    RSA *m_rsa;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_RSA_KEY_H
