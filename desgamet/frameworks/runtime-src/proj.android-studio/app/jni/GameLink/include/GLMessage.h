//
//  GLMessage.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//


#ifndef GLMessage_h
#define GLMessage_h

#include "GLTarget.h"

#define GL_MAXLEN_MSGTEXT                   (5 * 1024)
#define GL_MAXLEN_MSGEXTRA                  (5 * 1024)

namespace gamelink
{
    typedef enum
    {
        GLMessageTypeText,
        GLMessageTypeVoice,
        GLMessageTypeCustom,
    }GLMessageType;

    typedef enum
    {
        GLMessageStatusCreated,
        GLMessageStatusSending,
        GLMessageStatusSent,
        GLMessageStatusSendingFailed,
        
        GLMessageStatusReceived
    }GLMessageStatus; ///< enum gotye message status.

    /**
     * @summary: GLMessage encapsulates text/audio/image/userdata message used in GotyeAPI.
     */
    struct GLMessage
    {
        GLMessageType type;
        GLMessageStatus status;

        GLTarget sender;
        GLTarget receiver;   ///< sender&receiver

        unsigned id;
        unsigned long long date;  ///< the time since 1970/1/1 00:00:00, in nanoseconds
        
        std::string text;
        std::string extraText;

        unsigned duration;
        std::string url;
        std::string file;
        
        bool operator < (const GLMessage& other) const;
        bool operator > (const GLMessage& other) const;
        bool operator == (const GLMessage& other) const;
        
        GLMessage();
        GLMessage(GLTarget sender, GLTarget receiver, GLMessageType type = GLMessageTypeText);

        static GLMessage createMessage(GLTarget sender, GLTarget receiver);
        static GLMessage createAudioMessage(GLTarget sender, GLTarget receiver);
        static GLMessage createTextMessage(GLTarget sender, GLTarget receiver, const std::string& text);
    };
}

#endif/* defined(GLMessage_h) */
