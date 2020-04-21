// -------------------------------------------------
// BlinKit - Test Program
// -------------------------------------------------
//   File Name: AppDelegate.mm
// Description: AppDelegate class
//      Author: Ziming Li
//     Created: 2019-08-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#import "AppDelegate.h"
#import <BkCrawler/bk_app.h>

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching: (NSNotification *)aNotification {
    // Insert code here to initialize your application
}


- (void)applicationWillTerminate: (NSNotification *)aNotification
{
    BkFinalize();
}

@end
