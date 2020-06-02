// -------------------------------------------------
// BlinKit - openssl Library
// -------------------------------------------------
//   File Name: opensslconf.h
// Description: Configuration Header File
//      Author: Ziming Li
//     Created: 2020-06-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_OPENSSL_OPENSSLCONF_H
#define BLINKIT_OPENSSL_OPENSSLCONF_H

#pragma once

#include <openssl/opensslv.h>

#define OPENSSL_NO_ASYNC
#define OPENSSL_NO_CRYPTO_MDEBUG
#define OPENSSL_NO_CRYPTO_MDEBUG_BACKTRACE
#define OPENSSL_NO_DGRAM
#define OPENSSL_NO_EC_NISTP_64_GCC_128
#define OPENSSL_NO_ENGINE
#ifdef NDEBUG
#   define OPENSSL_NO_ERR
#endif
#define OPENSSL_NO_STDIO
#define OPENSSL_RAND_SEED_OS

#define OPENSSLDIR  ""

typedef unsigned int    RC4_INT;

#ifndef DECLARE_DEPRECATED
#   define DECLARE_DEPRECATED(f)
#endif

#define DEPRECATEDIN_1_2_0  DECLARE_DEPRECATED
#define DEPRECATEDIN_1_1_0  DECLARE_DEPRECATED
#define DEPRECATEDIN_1_0_0  DECLARE_DEPRECATED
#define DEPRECATEDIN_0_9_8  DECLARE_DEPRECATED

#ifndef OPENSSL_FILE
#   ifdef OPENSSL_NO_FILENAMES
#       define OPENSSL_FILE ""
#       define OPENSSL_LINE 0
#   else
#       define OPENSSL_FILE __FILE__
#       define OPENSSL_LINE __LINE__
#   endif
#endif

#ifdef _WIN32
#   ifdef _WIN64
#       define SIXTY_FOUR_BIT
#   else
#       define THIRTY_TWO_BIT
#   endif
#endif

#endif // BLINKIT_OPENSSL_OPENSSLCONF_H
