// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bk_file.h
// Description: BkFile Class
//      Author: Ziming Li
//     Created: 2020-06-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_BK_FILE_H
#define BLINKIT_BLINKIT_BK_FILE_H

#pragma once

#include "bk_def.h"

namespace BlinKit {

class BkFile
{
public:
    static int WriteContent(const BkPathChar *fileName, const void *data, size_t size);
private:
    BkFile(void) = delete;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_BK_FILE_H
