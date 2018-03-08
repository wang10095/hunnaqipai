 
#ifndef __UseJni2__com_game_UseJni2_JniCallCocos__  
#define __UseJni2__com_game_UseJni2_JniCallCocos__  
  
#include "cocos2d.h"  
#include <jni.h>  
  
extern "C"  
{  

	JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_ChoosePhoneCallback(JNIEnv * env, jclass jj, jstring num, jstring name);
    JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GalleryCameraCallback(JNIEnv *, jclass, jint, jstring);  
    JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_wxShareCallback(JNIEnv * env, jclass jj, jstring token);
    JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_WxLoginCallback(JNIEnv * env, jclass jj, jstring token);
    JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_WxPayelCallback(JNIEnv * env, jclass jj, jstring token);
	JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_StartWithUrlCallback(JNIEnv * env, jclass jj , jstring jstr,jstring jkey);
 	JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GaoDeCallback(JNIEnv * env, jclass jj, jstring token);
 	JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_BatteryCallback(JNIEnv * env, jclass jj, jstring token);
    JNIEXPORT void JNICALL Java_org_cocos2dx_lua_JniCallCocos_GpsCallback(JNIEnv * env, jclass jj, jstring token);
}  
  
#endif /* defined(__UseJni2__com_game_UseJni2_JniCallCocos__) */  
