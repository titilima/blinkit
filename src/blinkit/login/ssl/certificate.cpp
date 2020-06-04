// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: certificate.cpp
// Description: Certificate Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "certificate.h"

#include <openssl/pem.h>
#include "blinkit/common/bk_file.h"

namespace BlinKit {

static const int SERIAL_RAND_BITS = 159;

Certificate::Certificate(void) : m_cert(X509_new())
{
    ASN1_INTEGER *sno = ASN1_INTEGER_new();

    BIGNUM *bn = BN_new();
    BN_rand(bn, SERIAL_RAND_BITS, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    BN_to_ASN1_INTEGER(bn, sno);
    BN_free(bn);

    X509_set_serialNumber(m_cert, sno);

    ASN1_INTEGER_free(sno);
}

Certificate::Certificate(const std::string &pem) : m_cert(nullptr)
{
    BIO *bio = BIO_new_mem_buf(pem.data(), pem.length());
    m_cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
}

Certificate::~Certificate(void)
{
    if (nullptr != m_cert)
        X509_free(m_cert);
}

int Certificate::Save(const BkPathChar *fileName)
{
#ifndef NDEBUG
    ASSERT(m_signed);
#endif
    BIO *mem = BIO_new(BIO_s_mem());
    if (nullptr == mem)
    {
        ASSERT(nullptr != mem);
        return BK_ERR_UNKNOWN;
    }

    int r = PEM_write_bio_X509(mem, m_cert);
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

int Certificate::Sign(X509_REQ *req, EVP_PKEY *key, int days)
{
    X509_set_issuer_name(m_cert, X509_REQ_get_subject_name(req));

    X509_set_subject_name(m_cert, X509_REQ_get_subject_name(req));

    X509_gmtime_adj(X509_getm_notBefore(m_cert), 0);
    X509_time_adj_ex(X509_getm_notAfter(m_cert), days, 0, nullptr);

    X509_set_pubkey(m_cert, key);

    int r = X509_sign(m_cert, key, nullptr);
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
