  
#include "org_cocos2dx_lua_JniCallCocos.h"  
#include "AppDelegate.h"
// #include "PPCEX/helper/ToolsHelper.h"


USING_NS_CC;  
  
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GalleryCameraCallback(JNIEnv * env, jclass jj, jint type, jstring jstr)  
{  
    // 这里只是简单的打印一下日志，表示调用成功  
    // 这里用Cocos2d-x的消息中心发送消息给当前场景还是很方便滴  
    
    const char* str;  
    str = env->GetStringUTFChars(jstr,JNI_FALSE);  
    log("cocos GalleryCamera In C++ From Java %d %s ^_^",type,str);
    
    // ToolsHelper::dispatchEvent(type ,str);

    env->ReleaseStringUTFChars(jstr, str);
}  
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_StartWithUrlCallback(JNIEnv * env, jclass jj , jstring jstr,jstring jkey)
{
    
    const char* str;  
    str = env->GetStringUTFChars(jstr,JNI_FALSE);
    const char* key;
    key = env->GetStringUTFChars(jkey,JNI_FALSE);
    log("cocos StartWithUrlCallback In C++ From Java %s ^_^",str);
     AppDelegate::startWithUrl(str,key);
    env->ReleaseStringUTFChars(jstr, str);
    env->ReleaseStringUTFChars(jkey, key);

}
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_ChoosePhoneCallback(JNIEnv * env, jclass jj, jstring num, jstring name)
{
    const char* num_str;
    const char* name_str;
    num_str = env->GetStringUTFChars(num,JNI_FALSE);  
    name_str = env->GetStringUTFChars(name,JNI_FALSE);

    log("cocos ChoosePhoneCallback In C++ From Java  %s %s ^_^",num_str,name_str);
    // ContractHelper::choosePhoneCallback(num_str,name_str);
    env->ReleaseStringUTFChars(num, num_str);
    env->ReleaseStringUTFChars(name, name_str);
}  

//String token, String openid, String refresh_token
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_WxLoginCallback(JNIEnv * env, jclass jj, jstring token)
{
    const char* token_str;
    token_str = env->GetStringUTFChars(token,JNI_FALSE);  
    log("cocos WxLoginCallback In C++ From Java %s ^_^",token_str);
     AppDelegate::startWithUrl(token_str,"OUT_WECHAT_LOGIN");
    env->ReleaseStringUTFChars(token, token_str);
}
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GaoDeCallback(JNIEnv * env, jclass jj, jstring token)
{
    const char* token_str;
    token_str = env->GetStringUTFChars(token,JNI_FALSE);  
    log("cocos GaoDeCallback In C++ From Java  %s ^_^",token_str);
     AppDelegate::startWithUrl(token_str,"GET_GAODE_INFO");
    env->ReleaseStringUTFChars(token, token_str);
}
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_BatteryCallback(JNIEnv * env, jclass jj, jstring token)
{
    const char* token_str;
    token_str = env->GetStringUTFChars(token,JNI_FALSE);  
    log("cocos BatteryCallback In C++ From Java  %s ^_^",token_str);
    // AppDelegate::startWithUrl(token_str,"GET_BATTERY");
    env->ReleaseStringUTFChars(token, token_str);
    
}
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GpsCallback(JNIEnv * env, jclass jj, jstring token)
{
    const char* token_str;
    token_str = env->GetStringUTFChars(token,JNI_FALSE);  
    log("cocos GpsCallback In C++ From Java  %s ^_^",token_str);
    // AppDelegate::startWithUrl(token_str,"GET_GPS");
    env->ReleaseStringUTFChars(token, token_str);
}
void JNICALL Java_org_cocos2dx_lua_JniCallCocos_wxShareCallback(JNIEnv * env, jclass jj, jstring token)
{
    const char* token_str;
    token_str = env->GetStringUTFChars(token,JNI_FALSE);  
    log("cocos wxShareCallback In C++ From Java  %s ^_^",token_str);
    // AppDelegate::startWithUrl(token_str,"SHARE_ERWEIMA");
    env->ReleaseStringUTFChars(token, token_str);
}
