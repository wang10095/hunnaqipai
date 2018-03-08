//
//  GameLink.cpp
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#include "GameLink.h"

#if !defined(ANDROID)
#if !defined(WIN32)||defined(GAMELINK_WIN_STATIC)
#include "GotyeCAPI.h"
#else
#include <windows.h>
static  HINSTANCE dlhandle = NULL;
static void* (*gamelink_create)();
#endif
#else
#include "JniHelper.h"
#include <jni.h>
#include <dlfcn.h>
#define gamelink_create pfn_gamelink_create
static void* dlhandle = nullptr;
static void* (*gamelink_create)();
#endif
#if defined(WIN32)
#include <stdarg.h>
#endif

namespace gamelink
{

    IGameLink *GameLink::impl = nullptr;
    static std::vector<GLMessage> emptyMessageList;

    bool GameLink::init(const std::string& appKey)
    {
        if(createImpl()){
            return impl->init(appKey);
        }
        
        return false;
    }
    
    void GameLink::setCustomHost(const std::string& host, int port){
        if(impl){
            impl->setCustomHost(host, port);
        }
    }
    
    void GameLink::enableLog(bool enable){
        if(impl){
            impl->enableLog(enable);
        }
    }
    
    void GameLink::clearCache()
    {
        if(impl){
            impl->clearCache();
        }
    }
    
    void GameLink::enableVoiceRecognition(bool enable)
    {
        if(impl){
            impl->enableVoiceRecognition(enable);
        }
    }

    void GameLink::pump()
    {
        if(impl){
            impl->pump();
        }
    }
        
    void GameLink::removeAllObservers()
    {
        if(impl){
            impl->removeAllObservers();
        }
    }
    
    void GameLink::addObserver(const GLObserver& observer)
    {
        if(impl){
            impl->addObserver(observer);
        }
    }
    
    void GameLink::removeObserver(const GLObserver& observer)
    {
        if(impl){
            impl->removeObserver(observer);
        }
    }
    
    void GameLink::login(const std::string &account)
    {
        if(impl){
            impl->login(account);
        }
    }
    
    void GameLink::setProfile(const std::string& nickname, const std::string& avatar,  const std::string& extraInfo)
    {
        if(impl){
            impl->setProfile(nickname, avatar, extraInfo);
        }
    }
    
    void GameLink::logout()
    {
        if(impl){
            impl->logout();
        }
    }
    
    void GameLink::getPermanentChannels(unsigned pageIndex, unsigned countPerPage)
    {
        if(impl){
            impl->getPermanentChannels(pageIndex, countPerPage);
        }
    }

    void GameLink::joinChannel(const std::string& channel)
    {
        if(impl){
            impl->joinChannel(channel);
        }
    }
    
    void GameLink::joinChannel(const GLTarget& channel)
    {
        if(impl){
            impl->joinChannel(channel.account);
        }
    }
    
    void GameLink::leaveChannel(const std::string& channel)
    {
        if(impl){
            impl->leaveChannel(channel);
        }
    }
    
    void GameLink::leaveChannel(const GLTarget& channel)
    {
        if(impl){
            impl->leaveChannel(channel.account);
        }
    }
    
    void GameLink::joinIntercomOfChannel(const GLTarget& channel)
    {
        if(impl){
            impl->joinIntercomOfChannel(channel);
        }
    }
    
    void GameLink::quitIntercom()
    {
        if(impl){
            impl->quitIntercom();
        }
    }
    
    void GameLink::startTalk()
    {
        if(impl){
            impl->startTalk();
        }
    }
    
    void GameLink::stopTalk()
    {
        if(impl){
            impl->stopTalk();
        }
    }
    
    void GameLink::getChannelMemberList(const GLTarget& channel, unsigned pageIndex, unsigned countPerPage)
    {
        if(impl){
            impl->getChannelMemberList(channel, pageIndex, countPerPage);
        }
    }

    void GameLink::sendText(const GLTarget& receiver, const std::string& text, const std::string& extra)
    {
        if(impl){
            impl->sendText(receiver, text, extra);
        }
    }
    
    void GameLink::sendCustomText(const GLTarget& target, const std::string& customText, const std::string& extra)
    {
        if(impl){
            impl->sendCustomText(target, customText, extra);
        }
    }
    
    void GameLink::getHistoryMessageList(const GLTarget& target, unsigned count)
    {
        if(impl){
            impl->getHistoryMessageList(target.account, count);
        }
    }
    
    void GameLink::clearMessageList(const GLTarget& target)
    {
        if(impl){
            impl->clearMessageList(target);
        }
    }
    
    std::vector<GLMessage> GameLink::getMessageList(const GLTarget& target, unsigned count)
    {
        if(impl){
            std::vector<GLMessage> *plist = impl->getMessageList(target, count);
            if(plist){
                return *plist;
            }
        }
        
        return emptyMessageList;
    }
    
    void GameLink::startRecord()
    {
        if(impl){
            impl->startRecord();
        }
    }
    
    void GameLink::getRecordingState(float* volume, unsigned* duration)
    {
        if (impl) {
            impl->getRecordingState(volume, duration);
        }
    }
    
    void GameLink::stopRecord()
    {
        if(impl){
            impl->stopRecord();
        }
    }
    
    void GameLink::uploadVoice(const std::string& file)
    {
        if(impl){
            impl->uploadVoice(file);
        }
    }
    
    void GameLink::downloadVoice(const std::string& url)
    {
        if(impl){
            impl->downloadVoice(url);
        }
    }
    
    void GameLink::downloadVoice(const GLMessage& message)
    {
        if(impl){
            impl->downloadVoice(message);
        }
    }
    
    void GameLink::sendVoice(const GLTarget& receiver, const std::string& url, unsigned duration, const std::string& extra, const std::string& voiceContent)
    {
        if(impl){
            impl->sendVoice(receiver, url, duration, extra, voiceContent);
        }
    }
    
    void GameLink::playVoice(const std::string& file)
    {
        if(impl){
            impl->playVoice(file);
        }
    }
    
    void GameLink::playVoice(const GLMessage& message)
    {
        if(impl){
            impl->playVoice(message);
        }
    }
    
    void GameLink::getPlayingState(float* volume, unsigned* duration)
    {
        if (impl) {
            impl->getPlayingState(volume, duration);
        }
    }
    
    void GameLink::stopPlay()
    {
        if(impl){
            impl->stopPlay();
        }
    }
    
    const char* GameLink::getVersion()
    {
        if(impl){
            return impl->getVersion();
        }
        
        return "";
    }
    
    bool GameLink::loadDylib(){
#if defined(ANDROID)
        if(dlhandle){
            return true;
        }
        
        jstring jpackageName = nullptr;
        
        cocos2d::JniMethodInfo info;
        
        bool ret = cocos2d::JniHelper::getStaticMethodInfo(info, "me/gamelink/GameLink", "createInstance", "(Landroid/app/Activity;)Ljava/lang/String;");
        
        if(ret)
        {
            jobject activity = cocos2d::JniHelper::getActivity();
            jpackageName = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID, activity);
        }else{
            return false;
        }
        
        const char *cstr = info.env->GetStringUTFChars(jpackageName, 0);
        std::string cpackageName = cstr;
        info.env->ReleaseStringUTFChars (jpackageName, cstr);
        
        std::string path = "/data/data/" + cpackageName + "/lib/libGameLink.so";
        dlhandle = dlopen(path.c_str(), RTLD_LAZY);
        
        if(!dlhandle){
            return false;
        }
        
        gamelink_create = (void* (*)())dlsym(dlhandle, "gamelink_create");
        return gamelink_create != nullptr;
#elif defined(WIN32)&&!defined(GAMELINK_WIN_STATIC)
        dlhandle = ::LoadLibrary(TEXT("GameLink.dll"));
        gamelink_create = (void* (*)())::GetProcAddress(dlhandle, "gamelink_create");
        
        return gamelink_create != nullptr;
#else
        return true;
#endif
    }
    
    bool GameLink::createImpl(){
        if(!loadDylib()){
            return false;
        }
        
        if(!impl){
            impl = (IGameLink*)gamelink_create();
        }
        
        return (impl != nullptr);
    }
}
