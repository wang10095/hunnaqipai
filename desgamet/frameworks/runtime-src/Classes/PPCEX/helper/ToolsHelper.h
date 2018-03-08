//  
//  Created by liqing on 2015-Apr-13.  
//  
//  
  
#ifndef __MISC_HELPER__  
#define __MISC_HELPER__  

#include <string>
class ToolsHelper
{  
public:
    static void dispatchEvent(int type,const char* str);
    static void callOpenImageChoose(std::string uid);
    static void callOpenCamera();
    //weichat
    static void sendAppContent(std::string title,std::string content,std::string data,std::string icon);
    static void sendTextContent(std::string content);
    static void sendImageContent(std::string file, int scene);
    static void sendLinkContent(std::string title,std::string content,std::string url, int scene);
    static void sendWechatPay(std::string partnerId,std::string prepayId,std::string nonceStr,std::string timeStamp,std::string sign);
    static void sendLogin(std::string scope, std::string state);// 登陆
    static bool checkWechat();
    //push
    static void setAlias(std::string alias);
    static void setTags(std::string tags);
    static void delTags();
    static void checkVoice(); //检查麦克风
    static void getGaoDeInfo();
    static int getBattery();
    static bool checkGps();
    static void copy(std::string str);
    static void saveImage(std::string path);
};
#endif
