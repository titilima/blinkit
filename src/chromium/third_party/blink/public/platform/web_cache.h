// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_cache.h
// Description: WebCache Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WEB_CACHE_H
#define BLINKIT_BLINK_WEB_CACHE_H

#pragma once

namespace blink {

class WebCache
{
public:
    static void Clear(void);
private:
    WebCache(void) = delete;
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_CACHE_H
