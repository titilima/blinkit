// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: controller.cpp
// Description: Controller Related Implementations
//      Author: Ziming Li
//     Created: 2019-09-06
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "controller_impl.h"

extern "C" {

BKEXPORT int BKAPI BkControllerCancelWork(BkWorkController controller)
{
    return controller->CancelWork();
}

BKEXPORT int BKAPI BkControllerContinueWorking(BkWorkController controller)
{
    return controller->ContinueWorking();
}

BKEXPORT int BKAPI BkReleaseController(BkWorkController controller)
{
    return controller->Release();
}

} // extern "C"
