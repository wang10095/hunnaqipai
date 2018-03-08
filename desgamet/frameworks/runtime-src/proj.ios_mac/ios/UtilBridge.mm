//
//
//  Created by liqing on 2015Apr17
//
//

#include "UtilBridge.h"
#include "PickerDelegate.h"
#include "WXApi.h"
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import <AMapFoundationKit/AMapFoundationKit.h>
#import <AMapLocationKit/AMapLocationKit.h>
#import "JPUSHService.h"
#import "AppDelegate.h"
#define kWXAPP_ID_PAY @"wx0fd58048862fa7dc" //TeamPoker
#define DefaultLocationTimeout 10
#define DefaultReGeocodeTimeout 5
static AMapLocationManager* _loc = nil;
void UtilBridge::openImagePicker(ImagePickerMode mode)
{
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    //设置图像来源类型(先判断系统中哪种图像源是可用的)
    switch(mode)
    {
    case ImagePickerMode::FROM_CAMERA:
        if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
        {
            picker.sourceType = UIImagePickerControllerSourceTypeCamera;
        }else
        {
            return;
        }
        break;
    case ImagePickerMode::FROM_PHOTO:
        if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
        {
            picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        }else
        {
            return;
        }
        break;
    default:
        return;
    }

    PickerDelegate* delegate = [[PickerDelegate alloc] init];
    picker.delegate = delegate;
    picker.allowsEditing = YES;
    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:picker animated:YES completion:^{}];
}
void UtilBridge::openCamera(){
    openImagePicker(ImagePickerMode::FROM_CAMERA);
}

void UtilBridge::openImageChoose(){
    openImagePicker(ImagePickerMode::FROM_PHOTO);
}
void UtilBridge::sendAppContent(const char* title,const char* content,const char* data,const char* icon){
    NSString *titleText = [NSString stringWithUTF8String:title]; // no retain =[retain]
    NSString *contentText =[NSString stringWithUTF8String:content];
    NSString *dataText =[NSString stringWithUTF8String:data];
    NSString *iconText =[NSString stringWithUTF8String:icon];
    
    WXAppExtendObject* ext = [WXAppExtendObject object];
    ext.extInfo = dataText;
    WXMediaMessage* message = [WXMediaMessage message];
    message.title = titleText;
    message.description = contentText;
    message.mediaObject = ext;
    UIImage *image = [UIImage imageWithData:[NSData dataWithContentsOfFile:iconText]];
    [message setThumbImage:image];
//    [message setThumbImage:[UIImage imageNamed:@"res2.jpg"]];
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.message = message;
    req.scene = WXSceneSession;
    [WXApi sendReq:req];
}
void UtilBridge::sendTextContent(const char *content){
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.text = [NSString stringWithUTF8String:content];
    req.bText = YES;
    req.scene = WXSceneSession;
    [WXApi sendReq:req];
}
void UtilBridge::sendImageContent(const char *file, int scene){
    NSString *fileText = [NSString stringWithUTF8String:file];
//    NSData  *imageData = [[NSData alloc]initWithContentsOfFile:fileText];
//    UIImage *image   = [UIImage imageWithData:imageData];
//    imageData = UIImageJPEGRepresentation([UIImage imageWithData:imageData scale:0.1], 0.1f);
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:fileText];
    WXMediaMessage *mes = [WXMediaMessage message];
    mes.mediaObject = ext;
//    mes.thumbData = imageData;

    SendMessageToWXReq *req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.message = mes;
    req.scene = scene; //WXSceneSession;
    [WXApi sendReq:req];
}
void UtilBridge::sendLinkContent(const char* title,const char* content,const char* url,int scene){
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = [NSString stringWithUTF8String:url];
    WXMediaMessage *mes = [WXMediaMessage message];
    mes.title = [NSString stringWithUTF8String:title];
    mes.description = [NSString stringWithUTF8String:content];
    mes.mediaObject = ext;
    NSDictionary *infoPlist = [[NSBundle mainBundle] infoDictionary];
    NSString *icon = [[infoPlist valueForKeyPath:@"CFBundleIcons.CFBundlePrimaryIcon.CFBundleIconFiles"] lastObject];
    [mes setThumbImage:[UIImage imageNamed:icon]];
    SendMessageToWXReq *req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.message = mes;
    req.scene = scene;
    [WXApi sendReq:req];
}
void UtilBridge::sendWechatPay(const char *partnerId, const char *prepayId, const char *nonceStr, const char *timeStamp, const char *sign){
    PayReq* req             = [[[PayReq alloc] init]autorelease];
    req.openID              = kWXAPP_ID_PAY;
    req.partnerId           = [NSString stringWithUTF8String:partnerId];
    req.prepayId            = [NSString stringWithUTF8String:prepayId];
    req.nonceStr            = [NSString stringWithUTF8String:nonceStr];
    req.timeStamp           = [NSString stringWithUTF8String:timeStamp].intValue;
    req.package             = @"Sign=WXPay";
    req.sign                = [NSString stringWithUTF8String:sign];
    [WXApi sendReq:req];
}
void UtilBridge::sendLogin(const char *scope, const char *state){
    SendAuthReq* req        = [[[SendAuthReq alloc] init]autorelease];
    req.state               = [NSString stringWithUTF8String:state];
    req.scope               = [NSString stringWithUTF8String:scope];
    [WXApi sendReq:req];
}
bool UtilBridge::checkWechat(){
    return [WXApi isWXAppInstalled];
}
void UtilBridge::setAlias(const char* alias){
    NSString* alia = [NSString stringWithUTF8String:alias];
    [JPUSHService setAlias:alia completion:^(NSInteger iResCode, NSString *iAlias, NSInteger seq) {
        NSLog(@"UtilBridge: setAlias -> iResCode:%ld,iAlias:%@",iResCode,iAlias);
    } seq:0];
}
void UtilBridge::setTags(const char* tags) {
    NSString* tag = [NSString stringWithUTF8String:tags];
    NSArray * tagsList = [tag componentsSeparatedByString:@","];
    if (tagsList.count > 0){
        NSMutableSet * taga = [[NSMutableSet alloc] init];
        [taga addObjectsFromArray:tagsList];
        [JPUSHService setTags:taga completion:^(NSInteger iResCode, NSSet *iTags, NSInteger seq) {
            NSLog(@"UtilBridge: setTags -> iResCode:%ld",iResCode);
        } seq:1];
    }
}
void UtilBridge::delTags(){
    [JPUSHService cleanTags:^(NSInteger iResCode, NSSet *iTags, NSInteger seq) {
        NSLog(@"UtilBridge: delTags -> iResCode:%ld",iResCode);
    } seq:2];
}
void UtilBridge::checkVoice(){
    if ([[AVAudioSession sharedInstance] respondsToSelector:@selector(requestRecordPermission:)]){
        [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
            if (granted) {
                //用户已允许，直接进行录制等，逻辑操作...
            }else {
                //用户拒绝，引导用户开启麦克风服务，直接跳转到该应用设置页面
            }
        }];
    }
}
void UtilBridge::getGaodeLoc(){
    if(_loc == nil){
        NSString* bid = [[NSBundle mainBundle]bundleIdentifier];
        NSLog(@"UtilBridge: getGaodeLoc -> loc=nil %@",bid);
        if([bid isEqualToString:@"com.nabai.desgame"]){
            [AMapServices sharedServices].apiKey =@"d47f36628964454df691f71b8cc86873";
        }else if ([bid isEqualToString:@"com.nabai.desgamet"]){
            [AMapServices sharedServices].apiKey =@"04641032ccc18383abb627e42aaa34aa";
        }
        _loc = [[AMapLocationManager alloc] init];
        [_loc setLocationTimeout:DefaultLocationTimeout]; //设置定位超时时间 2
        [_loc setReGeocodeTimeout:DefaultReGeocodeTimeout];//设置逆地理超时时间 2
        [_loc setDesiredAccuracy:kCLLocationAccuracyNearestTenMeters]; //设置期望定位精度
        [_loc setPausesLocationUpdatesAutomatically:NO];//设置不允许系统暂停定位
    }
    [_loc requestLocationWithReGeocode:YES completionBlock:^(CLLocation *location, AMapLocationReGeocode *regeocode, NSError *error) {
        if (error)
        {
            NSLog(@"UtilBridge: getGaodeLoc -> locError:{%ld - %@};", (long)error.code, error.localizedDescription);
            AppDelegate::startWithUrl("0,0,,","GET_GAODE_INFO");
        }else{
            NSLog(@"UtilBridge: getGaodeLoc -> locSuccess:%@lat:%f;lon:%f",regeocode,location.coordinate.latitude, location.coordinate.longitude);
            NSString * str = [NSString stringWithFormat:@"%f,%f,%@,%@",location.coordinate.longitude,location.coordinate.latitude,regeocode.city,regeocode.district]; // 市 + 区
            const char* gaodeInfo = [str UTF8String];
            AppDelegate::startWithUrl(gaodeInfo,"GET_GAODE_INFO");
        }
    }];
}
bool UtilBridge::checkGps(){
    if ([CLLocationManager locationServicesEnabled] && ([CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorizedWhenInUse || [CLLocationManager authorizationStatus] == kCLAuthorizationStatusNotDetermined || [CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorized)){
        //定位功能可用
        return true;
    }else if ([CLLocationManager authorizationStatus] ==kCLAuthorizationStatusDenied) {
        //定位不能用
        
    }
    return false;
}
int UtilBridge::getBattery()
{
    [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    float battery =  [[UIDevice currentDevice] batteryLevel]  *  100 ;
    int bt = (int)battery;
//    NSString * str = [NSString stringWithFormat:@"%f",battery];
//    const char* batteryInfo = [str UTF8String];
    return bt;
//    NSString * key = [NSString stringWithUTF8String:"GET_BATTERY"];
//

//    const char* batteryKey =  [key UTF8String];
//    AppDelegate::startWithUrl(batteryInfo,batteryKey);
}
void UtilBridge::copy(const char* str) {
    //把char*转换成OC的NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:str encoding:NSUTF8StringEncoding];
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    //改变剪切板的内容
    pasteboard.string = nsMessage;
}
void UtilBridge::saveImage(const char* paths){
    NSString* path = [NSString stringWithUTF8String:paths];
    NSString *aPath3=[NSString stringWithFormat:@"%@/Documents/%@.jpg",NSHomeDirectory(),path];
    UIImage *image = [UIImage imageWithContentsOfFile:aPath3];
    UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);
}
