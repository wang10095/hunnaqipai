//
//
//  Created by liqing on 2015Apr17
//
//
 
#ifndef __OpenCamera__UtilBridge__
#define __OpenCamera__UtilBridge__
class UtilBridge
{
public:
    //wechat
    static void sendAppContent(const char* title,const char* content,const char* data,const char* icon);
    static void sendTextContent(const char* content);
    static void sendImageContent(const char* file,int scene);
    static void sendLinkContent(const char* title,const char* content,const char* url,int scene);
    static void sendWechatPay(const char* partnerId,const char* prepayId,const char* nonceStr,const char* timeStamp,const char* sign);
    static void sendLogin(const char* scope, const char* state);
    static bool checkWechat();
    //push
    static void setAlias(const char* alias); //别名
    static void setTags(const char* tags);   // 标签
    static void delTags();    //删除所有标签
    //else
    static void openCamera();
    static void openImageChoose();
    static void checkVoice();
    static void copy(const char* str);
    static void getGaodeLoc();
    static bool checkGps();
    static void saveImage(const char* path);
    static int getBattery();

private:
    enum class ImagePickerMode
    {
        FROM_CAMERA,
        FROM_PHOTO
    };
    static void openImagePicker(ImagePickerMode mode);
};

#endif
