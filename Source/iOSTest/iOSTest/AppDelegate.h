//
//  AppDelegate.h
//  iOSTest
//
//  Created by wk on 14-3-24.
//  Copyright (c) 2014年 Pixel Soft. All rights reserved.
//

#include <thread>
#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic) std::thread* logicThread;

@end
