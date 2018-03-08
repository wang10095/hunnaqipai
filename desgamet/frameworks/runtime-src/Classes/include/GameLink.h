//
//  GameLink.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#ifndef GameLink_h
#define GameLink_h

#include <iostream>
#include "IGameLink.h"

namespace gamelink
{
    class GameLink
    {
    public:
        static bool init(const std::string& appId);
        
        static void setCustomHost(const std::string& host, int port);
        
        static void enableLog(bool enable);
        
        static void clearCache();
        
        static void enableVoiceRecognition(bool enable);
        
        static void pump();
        
        static void removeAllObservers();
        static void addObserver(const GLObserver& observer);
        static void removeObserver(const GLObserver& observer);
        
        static void login(const std::string& account);
        static void setProfile(const std::string& nickname, const std::string& avatar = "", const std::string& extraInfo = "");
        static void logout();
        
        static void getPermanentChannels(unsigned pageIndex, unsigned countPerPage);
        
        static void joinChannel(const std::string& channel);
        static void joinChannel(const GLTarget& channel);
        static void leaveChannel(const std::string& channel);
        static void leaveChannel(const GLTarget& channel);
        
        static void joinIntercomOfChannel(const GLTarget& channel);
        static void quitIntercom();
        
        static void startTalk();
        static void stopTalk();
        
        static void getChannelMemberList(const GLTarget& room, unsigned pageIndex, unsigned countPerPage);
        
        static void sendText(const GLTarget& receiver, const std::string& text, const std::string& extra = "");
        static void sendCustomText(const GLTarget& target, const std::string& customText, const std::string& extra = "");
        
        static void getHistoryMessageList(const GLTarget& target, unsigned count);

        static void clearMessageList(const GLTarget& target);
        static std::vector<GLMessage> getMessageList(const GLTarget& target, unsigned count = 0);
        
        static void startRecord();
        static void getRecordingState(float* volume, unsigned* duration);
        static void stopRecord();
        
        static void uploadVoice(const std::string& file);
        static void downloadVoice(const std::string& url);
        static void downloadVoice(const GLMessage& message);
        
        static void sendVoice(const GLTarget& receiver, const std::string& url, unsigned duration, const std::string& extra = "", const std::string& voiceContent = "");
        
        static void playVoice(const std::string& file);
        static void playVoice(const GLMessage& message);
        
        static void getPlayingState(float* volume, unsigned* duration);
        static void stopPlay();

        static const char* getVersion();
        
    private:
        static IGameLink *impl;
        static bool loadDylib();
        static bool createImpl();
    };
    
}

#endif /* GameLink_h */
