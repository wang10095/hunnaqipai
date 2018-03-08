//
//
//  Created by liqing on 2015Apr17
//
//

#import "PickerDelegate.h"
#include "ToolsHelper.h"


@implementation PickerDelegate  


-(id) init
{
    if(self = [super init])
    {
    }
    return self;
}
-(UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize
{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize, image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width*scaleSize, image.size.height*scaleSize)];
    UIImage *scaledImage=UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    
    //处理数据
    UIImage* image;
    CGFloat scale;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
        image = [info valueForKey:UIImagePickerControllerOriginalImage];
        CGFloat lenth = (image.size.width > image.size.height) ? image.size.height : image.size.width;
        scale = 200/lenth;
    }else{
        image = [info valueForKey:UIImagePickerControllerEditedImage];
        scale = 0.3;
    }
    UIImage* simage = [self scaleImage:image toScale:scale];
    image = simage;
    
    //获得临时存储路径
    NSString *imageDocPath = NSTemporaryDirectory();
    NSString *imageDocName = nil;

    //把图片转成NSData类型的数据来保存文件
    NSData *data = nil;
    //先用JPEG解析
//    if ((data = UIImageJPEGRepresentation(image, 1.0))) {
//        imageDocName = [imageDocPath stringByAppendingPathComponent:@"tmp.jpg"];
//        NSLog(@"JPEG %lu", (unsigned long)[data length]);
//    }else {
//        //如果不成功再用PNG解析
//        imageDocName = [imageDocPath stringByAppendingPathComponent:@"tmp.png"];
//        data = UIImagePNGRepresentation(image);
//        NSLog(@"PNG %lu", (unsigned long)[data length]);
//    }
    
    // update 先用png 解析
    if ((data = UIImagePNGRepresentation(image))) {
        imageDocName = [imageDocPath stringByAppendingPathComponent:@"tmp.png"];
        NSLog(@"PNG %lu", (unsigned long)[data length]);
    }else{
        imageDocName = [imageDocPath stringByAppendingPathComponent:@"tmp.jpg"];
        data = UIImageJPEGRepresentation(image,1.0);
        NSLog(@"JPEG %lu", (unsigned long)[data length]);
    }
    
    //保存
    BOOL ret = [[NSFileManager defaultManager] createFileAtPath:imageDocName contents:data attributes:nil];
    if (ret) {  
        NSLog(@"文件创建成功: %@" ,imageDocName);
        ToolsHelper::dispatchEvent(1,[imageDocName UTF8String]);
    }else {
        NSLog(@"文件创建失败: %@" ,imageDocName);
    }
    [picker dismissViewControllerAnimated:YES completion:^{}];
//    [picker dismissModalViewControllerAnimated: YES];
    [picker release];
}


- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
//    [picker dismissModalViewControllerAnimated: YES];
    [picker dismissViewControllerAnimated:YES completion:^{}];
    [picker release];
}

@end