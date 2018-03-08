//
//  GLMessage.cpp
//  
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//
//

#include "GLMessage.h"

namespace gamelink
{
    GLMessage::GLMessage():
    type(GLMessageTypeText), status(GLMessageStatusCreated),
    date(0), id(0), text(""), extraText(""), duration(0), url(""), file(""){
    }

    GLMessage::GLMessage(GLTarget s, GLTarget r, GLMessageType t)
    :sender(s), receiver(r),
    type(t), status(GLMessageStatusCreated),
    date(0), id(0), text(""), extraText(""), duration(0), url(""), file(""){}
    
    GLMessage GLMessage::createMessage(GLTarget sender, GLTarget receiver){
        return GLMessage(sender, receiver);
    }
    
    GLMessage GLMessage::createAudioMessage(GLTarget sender, GLTarget receiver){
        GLMessage message(sender, receiver, GLMessageTypeVoice);
        return message;
    }

    GLMessage GLMessage::createTextMessage(GLTarget sender, GLTarget receiver, const std::string& text){
        
        GLMessage message(sender, receiver);
        message.text = text;
        
        return message;
    }

    bool GLMessage::operator < (const GLMessage& other) const {
        return date < other.date;
    }

    bool GLMessage::operator == (const GLMessage& other) const {
        return date == other.date;
    }
    
    bool GLMessage::operator > (const GLMessage& other) const {
        return date > other.date;
    }
}



