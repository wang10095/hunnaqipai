//
//  GLTarget.cpp
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#include "GLTarget.h"
#include <sstream>

namespace gamelink
{
    GLTarget::GLTarget():type(GLTargetTypeUser),account(""),nickname(""),extraInfo(""), avatar(""){}

    GLTarget::GLTarget(GLTargetType targetType, const std::string& username):type(targetType),account(username),nickname(""),extraInfo(""), avatar(""){}
    
    GLTarget::GLTarget(GLTargetType targetType, const char* username):type(targetType),account(username),nickname(""),extraInfo(""), avatar(""){}

    bool GLTarget::operator == (const GLTarget& other) const {
        if (type != other.type)
            return false;
        
        return (account.compare(other.account) == 0);
    }
}
