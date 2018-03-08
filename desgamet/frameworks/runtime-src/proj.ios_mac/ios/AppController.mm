/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
 Copyright (c) 2013-2017 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "JPUSHService.h"
#ifdef NSFoundationVersionNumber_iOS_9_x_Max
#import <UserNotifications/UserNotifications.h>
#endif
#define kWXAPP_ID @"wx0fd58048862fa7dc"
#define kWXAPP_SECRET @"5eac194c81a4d349c7e07e96c5acf3fb"
@interface AppController ()<JPUSHRegisterDelegate>

@end

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

/*
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_6_0
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return UIInterfaceOrientationMaskAll;
}
#endif
*/
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Use RootViewController to manage CCEAGLView
    _viewController = [[RootViewController alloc]init];
    _viewController.wantsFullScreenLayout = YES;
    

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //custom
    [WXApi registerApp:@"wx0fd58048862fa7dc"];
//#if COCOS2D_DEBUG
//#else
    JPUSHRegisterEntity * entity = [[JPUSHRegisterEntity alloc] init];
    entity.types = JPAuthorizationOptionAlert|JPAuthorizationOptionBadge|JPAuthorizationOptionSound;
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
        
    }
    [JPUSHService registerForRemoteNotificationConfig:entity delegate:self];
    NSString* jpushkey = @"639999a11891f68992694b1a";
    NSString* bid = [[NSBundle mainBundle] bundleIdentifier];
    if([bid isEqualToString:@"com.nabai.game"]){
        jpushkey = @"cf98ce71dad9f1a1202b7f0f";
    }
    [JPUSHService setupWithOption:launchOptions appKey:jpushkey
                          channel:nil
                 apsForProduction:YES
            advertisingIdentifier:nil];
    [JPUSHService registrationIDCompletionHandler:^(int resCode, NSString *registrationID) {
        if(resCode == 0){
            NSLog(@"registrationID获取成功：%@",registrationID);
            
        }
        else{
            NSLog(@"registrationID获取失败，code：%d",resCode);
        }
    }];
    [JPUSHService resetBadge];
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
//#endif
    //run the cocos2d-x game scene
    app->run();
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
    cocos2d::Application::getInstance()->applicationWillResignActive();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    cocos2d::Application::getInstance()->applicationDidBecomeActive();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [_viewController release];
    [super dealloc];
}
#endif

#pragma mark -
#pragma mark custom sdk
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url{
    return [WXApi handleOpenURL:url delegate:self];
}
-(BOOL)application:(UIApplication *)application openURL:(nonnull NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(nonnull id)annotation{
    return [WXApi handleOpenURL:url delegate:self];
}
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray *restorableObjects))restorationHandler {
    if ([userActivity.activityType isEqualToString:NSUserActivityTypeBrowsingWeb]) {
        NSURL *webpageURL = userActivity.webpageURL;
        NSString *host = webpageURL.host;
        if([host isEqualToString:@"dev.nabaiuf.com"]){
            NSString *param = [NSString stringWithFormat:@"%@",[webpageURL query]];
            NSLog(@"Universal Links url: %@",param);
            // todo
            BOOL isCode = [param hasPrefix:@"webCode"];
            if(isCode){
                AppDelegate::startWithUrl([param UTF8String], "OUT_START_APP");
            }
            return YES;
        }else{
            [[UIApplication sharedApplication] openURL:webpageURL];
        }
    }
    return YES;
}
-(void) onReq:(BaseReq*)req
{
    if([req isKindOfClass:[GetMessageFromWXReq class]]){
        NSLog(@"AppController:onReq -> GetMessageFromWXReq");
        
    } else if([req isKindOfClass:[ShowMessageFromWXReq class]]){
        
        ShowMessageFromWXReq* showMessageFromWXReq = (ShowMessageFromWXReq*) req;
        WXAppExtendObject* obj = showMessageFromWXReq.message.mediaObject;
        NSString* str = [NSString stringWithFormat:@"%@",obj.extInfo];
        NSLog(@"AppController:onReq -> ShowMessageFromWXReq =%@",str);
//        AppDelegate::startWithUrl([str UTF8String],"OUT_START_APP");
        
    } else if([req isKindOfClass:[LaunchFromWXReq class]]){
        NSLog(@"AppController:onReq -> LaunchFromWXReq");
        
    }
}
-(void)onResp:(BaseReq *)resp
{
    if ([resp isKindOfClass:[SendAuthResp class]]) {
        SendAuthResp *presp =(SendAuthResp*)resp;
        NSLog(@"AppController: onResp -> SendAuthResp: code:%@,state:%@,lang:%@,country:%@,errCode:%d",presp.code,presp.state,presp.lang,presp.country,presp.errCode);
        if (presp.errCode== 0) {
            NSString *code = presp.code;
            NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",kWXAPP_ID,kWXAPP_SECRET,code];
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                NSURL *zoneUrl = [NSURL URLWithString:url];
                NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
                NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (data) {
                        NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                        NSMutableString* data = [NSMutableString string];
                        [data appendString:[dic objectForKey:@"access_token"]];
                        [data appendString:@","];
                        [data appendString:[dic objectForKey:@"openid"]];
                        [data appendString:@","];
                        [data appendString:[dic objectForKey:@"refresh_token"]];
                        AppDelegate::startWithUrl([data UTF8String],"OUT_WECHAT_LOGIN");
                    }
                });
            });
        }
    } else if ([resp isKindOfClass:[PayResp class]]){
        NSLog(@"AppController:onResp -> PayResp");
        PayResp *presp = (PayResp*)resp;
        switch (presp.errCode) {
            case WXSuccess:
                NSLog(@"支付成功－PaySuccess，retcode = %d", presp.errCode);
//                AppDelegate::startWithUrl("0","OUT_PAY_APP");
                break;
            default:
//                AppDelegate::startWithUrl("1","OUT_PAY_APP");
                NSLog(@"错误，retcode = %d, retstr = %@", presp.errCode,presp.errStr);
                break;
        }
        
    }else if([resp isKindOfClass:[SendMessageToWXResp class]]){
        SendMessageToWXResp *wxresp = (SendMessageToWXResp*)resp;
        if(wxresp.errCode == 0){
            NSLog(@"AppController:onResp -> 11111111");
//            AppDelegate::startWithUrl("1","SHARE_ERWEIMA");
            
        }else{
            NSLog(@"AppController:onResp -> 22222");
        }
    }
    else if ([resp isKindOfClass:[SendMessageToWXReq class]]){
        NSLog(@"AppController:onResp -> SendMessageToWXReq");
    }
}
#pragma mark- JPUSHRegisterDelegate
- (void)application:(UIApplication *)application
didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    
    /// Required - 注册 DeviceToken
    [JPUSHService registerDeviceToken:deviceToken];
}
// iOS 10 Support
- (void)jpushNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(NSInteger))completionHandler {
    // Required
    NSDictionary * userInfo = notification.request.content.userInfo;
    if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
        [JPUSHService handleRemoteNotification:userInfo];
    }
    completionHandler(UNNotificationPresentationOptionAlert); // 需要执行这个方法，选择是否提醒用户，有Badge、Sound、Alert三种类型可以选择设置
}

// iOS 10 Support
- (void)jpushNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler {
    // Required
    NSDictionary * userInfo = response.notification.request.content.userInfo;
    if([response.notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
        [JPUSHService handleRemoteNotification:userInfo];
    }
    completionHandler();  // 系统要求执行这个方法
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    
    // Required, iOS 7 Support
    [JPUSHService handleRemoteNotification:userInfo];
    completionHandler(UIBackgroundFetchResultNewData);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    
    // Required,For systems with less than or equal to iOS6
    [JPUSHService handleRemoteNotification:userInfo];
}
@end
