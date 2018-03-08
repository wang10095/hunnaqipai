#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "cocos2d.h"
#include "GameLink.h"
using namespace gamelink;
/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class  AppDelegate : private cocos2d::Application,private cocos2d::Ref,public GLObserver
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
    //custom
    virtual void applicationWillResignActive();
    virtual void applicationDidBecomeActive();
    static void startWithUrl(const char* str,const char* key);
    void noticeGotye(std::string ,unsigned,unsigned);
    void eventGotye(std::string);
    void gotyePlayer();
    // sdk 用户登录 登出
    virtual void onLogin(const GLTarget& user,const GLError* error);
    virtual void onLogout(const GLError* error);
    
    // sdk 用户加入频道，离开频道
    virtual void onJoinChannel(const GLTarget& channel, const GLError* error);
    virtual void onLeaveChannel(const GLTarget& room, const GLError* error);
    
    // sdk 用户发送消息，接收消息，下载消息
    virtual void onSendMessage(const GLMessage& message, const GLError* error);
    virtual void onReceiveMessage(const gamelink::GLMessage& message);
    virtual void onDownloadVoice(const GLMessage& message, const GLError* error);
    virtual void onUploadVoice(const std::string& file, const std::string& url, const GLError* error);
    // sdk 用户播放消息，播放中，播放停止
    virtual void onPlayStart(const GLMessage &message, const GLError* error);
    virtual void onPlaying(const GLMessage &message, unsigned duration);
    virtual void onPlayStop(const GLMessage &message);
    
    // sdk 用户录音开始，录音中，录音结束
    virtual void onStartRecord(const GLError* error);
    virtual void onRecording(float volume, unsigned duration);
    virtual void onStopRecord(const std::string& file, const std::string& voiceContent, unsigned duration, const GLError* error);
private:
    cocos2d::EventCustom* _gotyeEvent;
    cocos2d::EventCustom* _gotyeNotice;
    cocos2d::EventDispatcher* _gotyeDispatcher;
    bool _isPlay;
    bool _isRecord;
    std::string _name;
    std::queue<GLMessage> _gotyeMessage;
};

#endif  // __APP_DELEGATE_H__

