//
//  GLObserver.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#ifndef GLObserver_h
#define GLObserver_h

#include "GLError.h"
#include "GLMessage.h"

#define Optional    {}

namespace gamelink
{
    class GLObserver
    {
    public:

        virtual void onLogin(const GLTarget& user,const GLError* error) Optional;
        virtual void onLogout(const GLError* error) Optional;
        
        virtual void onGetPermanentChannels(unsigned pageIndex, const std::vector<GLTarget>& channels, const GLError* error) Optional;
        virtual void onJoinChannel(const GLTarget& channel, const GLError* error) Optional;
        virtual void onLeaveChannel(const GLTarget& channel, const GLError* error) Optional;
        
        virtual void onJoinIntercomOfChannel(const GLTarget& channel, const GLError* error) Optional;
        virtual void onQuitIntercom(const GLError* error) Optional;
        
        virtual void onStartTalk(const GLTarget& user, const GLError* error) Optional;
        virtual void onStopTalk(const GLTarget& user, const GLError* error) Optional;

        virtual void onGetChannelMemberList(const GLTarget& channel, unsigned pageIndex, unsigned total, const std::vector<std::string>& memberlist, const GLError* error) Optional;
        virtual void onGetHistoryMessageList(const GLTarget& target, unsigned count, const GLError* error) Optional;
        
        virtual void onSendMessage(const GLMessage& message, const GLError* error) Optional;
        virtual void onReceiveMessage(const GLMessage& message) Optional;
        
        virtual void onStartRecord(const GLError* error) Optional;
        virtual void onStopRecord(const std::string& file, const std::string& voiceContent, unsigned duration, const GLError* error) Optional;
        
        virtual void onUploadVoice(const std::string& file, const std::string& url, const GLError* error) Optional;
        virtual void onDownloadVoice(const std::string& url, const std::string& file, const GLError* error) Optional;
        virtual void onDownloadVoice(const GLMessage& message, const GLError* error) Optional;

        virtual void onPlayStart(const GLMessage &message, const GLError* error) Optional;
        virtual void onPlayStop(const GLMessage &message) Optional;
        
        virtual void onPlayStart(const std::string& file, const GLError* error) Optional;
        virtual void onPlayStop(const std::string& file) Optional;
    };
}

#endif
