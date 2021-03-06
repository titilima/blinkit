// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: sign_request.cpp
// Description: SignRequest Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "sign_request.h"

#include "bk_def.h"

namespace BlinKit {

SignRequest::SignRequest(void) : m_req(X509_REQ_new())
{
    X509_REQ_set_version(m_req, 0);
}

SignRequest::~SignRequest(void)
{
    X509_REQ_free(m_req);
}

void SignRequest::SetCommonName(const char *commonName)
{
    char *cn = const_cast<char *>(commonName); // Make compatible with old versions of OpenSSL.

    X509_NAME *n = X509_NAME_new();
    X509_NAME_add_entry_by_NID(n, NID_commonName, MBSTRING_UTF8,
        reinterpret_cast<unsigned char *>(cn), strlen(commonName),
        -1, 0);
    X509_REQ_set_subject_name(m_req, n);
    X509_NAME_free(n);
}

int SignRequest::Sign(EVP_PKEY *key)
{
#ifndef NDEBUG
    ASSERT(!m_signed);
    m_signed = true;
#endif

    X509_REQ_set_pubkey(m_req, key);

    int r;
    EVP_MD_CTX *mctx = EVP_MD_CTX_create();
    EVP_DigestSignInit(mctx, nullptr, nullptr, nullptr, key);
    r = X509_REQ_sign_ctx(m_req, mctx);
    EVP_MD_CTX_destroy(mctx);

    if (0 == r)
    {
        ASSERT(0 != r);
        return BK_ERR_UNKNOWN;
    }
#ifndef NDEBUG
    m_signed = true;
#endif
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
