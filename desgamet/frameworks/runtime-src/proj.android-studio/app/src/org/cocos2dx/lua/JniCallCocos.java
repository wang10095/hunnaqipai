package org.cocos2dx.lua;  

// 需回调给c++层的方法
public class JniCallCocos
{
    public static native void GalleryCameraCallback(int type, String str);  
	public static native void StartWithUrlCallback(String str, String key);
	public static native void ChoosePhoneCallback(String num, String name);  
	public static native void WxLoginCallback(String token);
	public static native void GaoDeCallback(String str);
	public static native void BatteryCallback(String str);
	public static native void GpsCallback(String str);
	public static native void wxShareCallback(String str);
}  
