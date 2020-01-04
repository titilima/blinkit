// -------------------------------------------------
// BlinKit - net Library
// -------------------------------------------------
//   File Name: net_util.cpp
// Description: Net Utilities
//      Author: Ziming Li
//     Created: 2018-10-21
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "net_util.h"

#include "base/logging.h"
#include "url/url_canon_stdstring.h"

namespace net {

std::string CanonicalizeHost(const std::string &host, url::CanonHostInfo *host_info)
{
    // Try to canonicalize the host.
    const url::Component rawHostComponent(0, static_cast<int>(host.length()));
    std::string canonHost;
    url::StdStringCanonOutput canonHostOutput(&canonHost);
    url::CanonicalizeHostVerbose(host.c_str(), rawHostComponent, &canonHostOutput, host_info);

    if (host_info->out_host.is_nonempty() && host_info->family != url::CanonHostInfo::BROKEN)
    {
        // Success!  Assert that there's no extra garbage.
        canonHostOutput.Complete();
        DCHECK_EQ(host_info->out_host.len, static_cast<int>(canonHost.length()));
    }
    else
    {
        // Empty host, or canonicalization failed.  We'll return empty.
        canonHost.clear();
    }

    return canonHost;
}

} // namespace net
