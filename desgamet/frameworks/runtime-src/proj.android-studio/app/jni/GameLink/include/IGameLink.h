//
//  IGameLink.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#ifndef IGameLink_h
#define IGameLink_h

#include <vector>
#include <iostream>
#include "GLMessage.h"
#include "GLObserver.h"


namespace gamelink
{
    class IGameLink
    {
    public:
        virtual bool init(const std::string& appKey) = 0;
        
        virtual void enableLog(bool enable) = 0;
        
        virtual void clearCache() = 0;

        virtual void enableVoiceRecognition(bool enable) = 0;
        
        virtual void pump() = 0;
        virtual void log(const std::string& info) = 0;
        virtual void setCustomHost(const std::string& host, int port) = 0;
        
        virtual void removeAllObservers() = 0;
        virtual void addObserver(const GLObserver& observer) = 0;
        virtual void removeObserver(const GLObserver& observer) = 0;

        virtual void login(const std::string& account) = 0;
        virtual void setProfile(const std::string& nickname, const std::string& avatar,  const std::string& extraInfo) = 0;
        virtual void logout() = 0;
        
        virtual void getPermanentChannels(unsigned pageIndex, unsigned countPerPage) = 0;
        
        virtual void joinChannel(const std::string& channel) = 0;
        virtual void leaveChannel(const std::string& channel) = 0;
        
        virtual void joinIntercomOfChannel(const GLTarget& channel) = 0;
        virtual void quitIntercom() = 0;
        
        virtual void startTalk() = 0;
        virtual void stopTalk() = 0;
        
        virtual void getChannelMemberList(const GLTarget& room, unsigned pageIndex, unsigned countPerPage) = 0;
        
        virtual void sendText(const GLTarget& receiver, const std::string& text, const std::string& extra) = 0;
        virtual void sendCustomText(const GLTarget& target, const std::string& customText, const std::string& extra) = 0;
        
        virtual void getHistoryMessageList(const std::string& channel, unsigned count) = 0;

        virtual void clearMessageList(const GLTarget& target) = 0;
        virtual std::vector<GLMessage> *getMessageList(const GLTarget& target, unsigned count) = 0;
        
        virtual void startRecord() = 0;
        virtual void getRecordingState(float* volume, unsigned* duration) = 0;
        virtual void stopRecord() = 0;
        
        virtual void uploadVoice(const std::string& file) = 0;
        virtual void downloadVoice(const std::string& url) = 0;
        virtual void downloadVoice(const GLMessage& message) = 0;
        
        virtual void sendVoice(const GLTarget& receiver, const std::string& url, unsigned duration, const std::string& extra, const std::string& voiceContent) = 0;
        
        virtual void playVoice(const std::string& file) = 0;
        virtual void playVoice(const GLMessage& message) = 0;
        
        virtual void getPlayingState(float* volume, unsigned* duration) = 0;
        virtual void stopPlay() = 0;

        
        virtual const char* getVersion() = 0;
    };
}

#endif /* IGameLink_h */
