//
//  GLTarget.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//


#ifndef GLTarget_h
#define GLTarget_h

#include <string>

#define GL_MAXLEN_ACCOUNT                   (0x40)
#define GL_MAXLEN_NICKNAME                  (0x40)
#define GL_MAXLEN_AVATAR                    (0x80)
#define GL_MAXLEN_EXTRAINFO                 (0xff)


namespace gamelink
{
    typedef enum
    {
        GLTargetTypeUser,
        GLTargetTypeChannel,
    }GLTargetType;
        

    struct GLTarget
    {
        GLTargetType type;
        
        std::string account;
        std::string avatar;
        std::string nickname;
        std::string extraInfo;
        
        GLTarget();
        GLTarget(GLTargetType type, const char* username);
        GLTarget(GLTargetType type, const std::string& username);
        
        bool operator==(const GLTarget& target) const;
    };
}

#endif/* defined(GLTarget_h) */
