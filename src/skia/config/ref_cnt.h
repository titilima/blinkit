// -------------------------------------------------
// BlinKit - skia Library
// -------------------------------------------------
//   File Name: ref_cnt.h
// Description: SkRefCnt Class
//      Author: Ziming Li
//     Created: 2018-07-14
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SKIA_REF_CNT_H
#define BLINKIT_SKIA_REF_CNT_H

#pragma once

class SK_API SkRefCnt : public SkRefCntBase
{
public:
    void deref(void) const { SkRefCntBase::unref(); }
};

#endif // BLINKIT_SKIA_REF_CNT_H
