//
//  GLError.h
//
//
//  Copyright © 2016 GameLink Team. All rights reserved.
//

#ifndef GLError_h
#define GLError_h

#include <iostream>

namespace gamelink
{

typedef enum
{
    GLErrorCodeNoError,
    GLErrorCodeRemoteException,
    GLErrorCodeNotInited = 0x100,
    GLErrorCodeNotOnline,
    GLErrorCodeInvalidArgument,
    
    GLErrorCodeKicked,
    GLErrorCodeNetworkError,
    GLErrorCodeAppSwitchedToBack,
    GLErrorCodeMicphoneAccessFailed,
    GLErrorCodeMicphoneBusy,
    
    GLErrorCodeUploadFailure,
    GLErrorCodeDownloadFailure,
    GLErrorCodePlaybackError,
    GLErrorCodeRecordDurationTooShort,
    GLErrorCodeTransferNotInited,
    
    GLErrorCodeFileNotExist,
    GLErrorCodeFileZeroBytes,
    GLErrorCodeMessageNotFound,
    GLErrorCodeOnlineOrLogining,
    GLErrorCodeNotInChannel,
    
    GLErrorCodeInOtherIntercom,
}GLErrorCode;

typedef struct
{
    GLErrorCode code;
    std::string detail;
    std::string description;
    
    int line;
    std::string function;
    std::string filename;
}GLError;
    
}

#endif /* GLErrorCode_h */
