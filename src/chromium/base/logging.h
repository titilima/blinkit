// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: logging.h
// Description: Placeholders for Logging
//      Author: Ziming Li
//     Created: 2019-02-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_LOGGING_H
#define BLINKIT_BASE_LOGGING_H

#pragma once

#ifdef _DEBUG
#   define DCHECK_IS_ON()   1
#else
#   define DCHECK_IS_ON()   0
#endif

#endif // BLINKIT_BASE_LOGGING_H
