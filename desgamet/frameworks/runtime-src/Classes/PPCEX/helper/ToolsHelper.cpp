//  
//  
//  Created by liqing on 2015-Apr-13.  
//  
//  
#include "cocos2d.h"  
#include "ToolsHelper.h"
#include "stdio.h"
#include <map>
#include <set>
USING_NS_CC;
static std::string _myUid = "";
void ToolsHelper::dispatchEvent(int type,const char* str)
{
    std::string name;
    switch(type){
        case 1: name = "chooseImage"; break;
        case 2: name = "desai.qrcode"; break;
        default: return;
    }
    cocos2d::EventCustom event(name);
    event.setData(std::string(str));
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    // 这样写，不能是多级目录，需先创建文件夹
//    CCLOG("cur user uid = %s",_myUid.c_str());
//    cocos2d::FileUtils::getInstance()->renameFile(std::string(str), cocos2d::FileUtils::getInstance()->getWritablePath()+"users/u_"+_myUid+".png");
//    cocos2d::FileUtils::getInstance()->renameFile(std::string(str), cocos2d::FileUtils::getInstance()->getWritablePath()+"users/"+_myUid+".jpg");
//    cocos2d::FileUtils::getInstance()->renameFile(std::string(str), cocos2d::FileUtils::getInstance()->getWritablePath()+"c.png");
    
}
/*
 * 安卓平台
 */
#pragma mark -
#pragma mark APPLE apk
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
void ToolsHelper::callOpenCamera(){
    
}
void ToolsHelper::sendAppContent(std::string title,std::string content,std::string data,std::string icon){
    
}
void ToolsHelper::sendTextContent(std::string content){
    JniMethodInfo minfo;
    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","sendTextContent", "(Ljava/lang/String;)V");
    
    if (!isHave) {
        log("cocos ToolsHelper::sendTextContent -> is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(content.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr);
    }
}
void ToolsHelper::sendLinkContent(std::string title,std::string content,std::string url, int scene){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","sendLinkContent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
    
    if (!isHave) {
        log("cocos ToolsHelper::sendImageContent is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(title.c_str());
        jstring jStr2 = minfo.env->NewStringUTF(content.c_str());
        jstring jStr3 = minfo.env->NewStringUTF(url.c_str());
        jint scene2 = 0;
        if (scene != 0){
            scene2 = scene;
        }
        //调用此函数
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr,jStr2,jStr3,scene2);
    }
}
bool ToolsHelper::checkWechat(){

}
void ToolsHelper::callOpenImageChoose(std::string uid){
    _myUid = uid;
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lua/AppActivity", "openImageChoose", "()V");
    if (isHave)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
void ToolsHelper::sendImageContent(std::string file, int scene){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","sendWechatImage", "(Ljava/lang/String;I)V");
    if (!isHave) {
        log("COCOS ToolsHelper::sendImageContent is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(file.c_str());
        jint scene2 = 0;
        if (scene != 0){
            scene2 = scene;
        }
        //调用此函数
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr,scene2);
    }
}
void ToolsHelper::sendWechatPay(std::string partnerId,std::string prepayId,std::string nonceStr,std::string timeStamp,std::string sign){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","sendWechatPay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    
    if (!isHave) {
        log("COCOS ToolsHelper::sendWechatPay is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(partnerId.c_str());
        jstring jStr2 = minfo.env->NewStringUTF(prepayId.c_str());
        jstring jStr3 = minfo.env->NewStringUTF(nonceStr.c_str());
        jstring jStr4 = minfo.env->NewStringUTF(timeStamp.c_str());
        jstring jStr5 = minfo.env->NewStringUTF(sign.c_str());
        //调用此函数
         log("COCOS ToolsHelper::sendWechatPay is succeed");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr,jStr2,jStr3,jStr4,jStr5);
    }
}
void ToolsHelper::sendLogin(std::string scope, std::string state){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","sendWechatLogin", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!isHave) {
        log("COCOS ToolsHelper::sendWechatLogin is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(scope.c_str());
        jstring jStr2 = minfo.env->NewStringUTF(state.c_str());
        //调用此函数
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr,jStr2);
    }
}
void ToolsHelper::setAlias(std::string alias){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","setAlias", "(Ljava/lang/String;)V");
    if (!isHave) {
        log("COCOS ToolsHelper::setAlias -> is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(alias.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr);
    }
}
void ToolsHelper::setTags(std::string tags){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","setTags", "(Ljava/lang/String;)V");
    if (!isHave) {
        log("COCOS ToolsHelper::setTags -> is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(tags.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jStr);
    }
}
void ToolsHelper::delTags(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","delTags", "()V");
    if (!isHave) {
        log("COCOS ToolsHelper::delTags -> is null");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
void ToolsHelper::checkVoice(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","checkVoice", "()V");
    if (!isHave) {
        log("COCOS ToolsHelper::checkVoice -> is null");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
void ToolsHelper::getGaoDeInfo(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","getGaodeLoc", "()V");
    if (!isHave) {
        log("COCOS ToolsHelper::getGaoDeInfo -> is null");
    }else{
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

int ToolsHelper::getBattery(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","getBattery", "()I");
    int bat = 0;
    if (!isHave) {
        log("COCOS ToolsHelper::getBattery -> is null");
    }else{
        bat = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
    }
    return bat;
}

bool ToolsHelper::checkGps(){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","checkGps", "()Z");
    bool gps = false;
    if (!isHave) {
        log("COCOS ToolsHelper::checkGps -> is null");
    }else{
        gps = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    }
    return gps;
}
void ToolsHelper::copy(std::string str){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","CopyText", "(Ljava/lang/String;)V");
    if (!isHave) {
        log("COCOS ToolsHelper::copy -> is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(str.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jStr);
    }
}
void ToolsHelper::saveImage(std::string path){
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/lua/AppActivity","imageToSavePhotos", "(Ljava/lang/String;)V");
    if (!isHave) {
        log("COCOS ToolsHelper::saveImage -> is null");
    }else{
        jstring jStr = minfo.env->NewStringUTF(path.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jStr);
    }
}
/*
 * 苹果平台
 */
#pragma mark -
#pragma mark APPLE ios
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "UtilBridge.h"
void ToolsHelper::callOpenImageChoose(std::string uid){
    _myUid = uid;
    UtilBridge::openImageChoose();
}
void ToolsHelper::callOpenCamera(){
    UtilBridge::openCamera();
}
void ToolsHelper::sendAppContent(std::string title, std::string content, std::string data,std::string icon){
    UtilBridge::sendAppContent(title.c_str(), content.c_str(), data.c_str(),icon.c_str());
}
void ToolsHelper::sendTextContent(std::string content){
    UtilBridge::sendTextContent(content.c_str());
}
void ToolsHelper::sendImageContent(std::string file, int scene){
    UtilBridge::sendImageContent(file.c_str(),scene);
}
void ToolsHelper::sendLinkContent(std::string title,std::string content,std::string url, int scene){
    UtilBridge::sendLinkContent(title.c_str(), content.c_str(), url.c_str(), scene);
}
void ToolsHelper::sendWechatPay(std::string partnerId,std::string prepayId,std::string nonceStr,std::string timeStamp,std::string sign){
    UtilBridge::sendWechatPay(partnerId.c_str(),prepayId.c_str(),nonceStr.c_str(),timeStamp.c_str(),sign.c_str());
}
void ToolsHelper::sendLogin(std::string scope, std::string state){
    UtilBridge::sendLogin(scope.c_str(), state.c_str());
}
bool ToolsHelper::checkWechat(){
    return UtilBridge::checkWechat();
}
void ToolsHelper::setAlias(std::string alias){
    UtilBridge::setAlias(alias.c_str());
}
void ToolsHelper::setTags(std::string tags){
    UtilBridge::setTags(tags.c_str());
}
void ToolsHelper::delTags(){
    UtilBridge::delTags();
}
void ToolsHelper::checkVoice(){
    UtilBridge::checkVoice();
}
void ToolsHelper::getGaoDeInfo(){
    UtilBridge::getGaodeLoc();
}
int ToolsHelper::getBattery(){
    return UtilBridge::getBattery();
}
bool ToolsHelper::checkGps(){
    return UtilBridge::checkGps();
}
void ToolsHelper::copy(std::string str){
    UtilBridge::copy(str.c_str());
}
void ToolsHelper::saveImage(std::string path){
    UtilBridge::saveImage(path.c_str());
}
#else
/*
 * 其他平台
 */
#endif
