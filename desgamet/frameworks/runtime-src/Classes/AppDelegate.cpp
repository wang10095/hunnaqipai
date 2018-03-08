#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "PPCEX/Common.h"
#include "PPCEX/pbc/pbc_lua.h"
#include "PPCEX/lsqlite3/lsqlite3.h"
#include "reader/CreatorReaderBinding.h"
#include "UpdateScene.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
extern "C"{
#endif
    int luaopen_tea(lua_State *L);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
}
#endif

// #define USE_AUDIO_ENGINE 1
 #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;
using namespace std;
AppDelegate::AppDelegate()
{
    _gotyeEvent = new EventCustom("gotyeEvent");    // 一般事件通知
    _gotyeNotice = new EventCustom("gotyeNotice");  // 播放声音通知
    _gotyeDispatcher = Director::getInstance()->getEventDispatcher();
    _isPlay = false;   // 没有播放声音
    _isRecord = false; // 没有录制声音
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif
    delete _gotyeEvent;
    delete _gotyeNotice;
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}
// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("COCOS AppDelegate::applicationDidEnterBackground()");
    EventCustom event("didEnterBackground");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("COCOS AppDelegate::applicationWillEnterForeground()");
    EventCustom event("willEnterForeground");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
#endif
}
void AppDelegate::applicationWillResignActive(){
    CCLOG("COCOS AppDelegate::applicationWillResignActive()+didEnterBackground");
    EventCustom event("didEnterBackground");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void AppDelegate::applicationDidBecomeActive(){
    CCLOG("COCOS AppDelegate::applicationDidBecomeActive()+willEnterForeground");
    EventCustom event("willEnterForeground");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
/*
 * custom
 */
void AppDelegate::startWithUrl(const char* str,const char* key)
{
    EventCustom event((std::string(key)));
    event.setData(std::string(str));
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
std::string s_group = "";
int gotyeLogin(lua_State * l) // 登录
{
    string name = (string)lua_tostring(l, 1);
    string group = (string)lua_tostring(l, 2);
    CCLOG("COCOS AppDelegate gamelink login user id:%s",name.c_str());
    GameLink::login(name);
    s_group = group;
    return 1;
}
void AppDelegate::onLogin(const GLTarget& user,const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_Login Fail code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        eventGotye("GY_Login"+error->detail);
        if(error->code == 272){
            
        }else{
            return;
        }
    }
    CCLOG("COCOS AppDelegate gamelink On_Login Succ accout= %s",user.account.c_str());
    _name = user.account;
    GameLink::joinChannel(s_group);
    eventGotye("GY_LoginOk");
}
int gotyeTalk(lua_State * l) // 开始录音
{
    GameLink::startRecord();
    return 1;
}
void AppDelegate::onStartRecord(const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_StartRecord code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        if (error->code == 262){
            eventGotye("GY_StartRecord_p");
        }
        return;
    }
    CCLOG("COCOS AppDelegate gamelink On_StartRecord Success");
    //to do
    _isRecord = true;
    eventGotye("GY_StartRecord");
}
void AppDelegate:: onRecording(float volume, unsigned duration)
{
    CCLOG("COCOS AppDelegate gamelink On_Recording volume = %f,duration = %d",volume,duration);
}
bool _isSend = true;
int gotyeStop(lua_State * l) //录音停止
{
    int isSend = lua_tonumber(l, 1);
    _isSend = isSend == 0 ? false : true;
    CCLOG("COCOS AppDelegate gamelink gotye_Stop isSend = %d",isSend);
    GameLink::stopRecord();
    return 1;
}
unsigned s_duration;
std::string s_voiceContent;
void AppDelegate::onStopRecord(const std::string& file, const std::string& voiceContent, unsigned duration, const GLError* error)
{
    if (error) {
        CCLOG("COCOS AppDelegate gamelink On_StopRecord code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        eventGotye("GY_StooRecordError");
        return;
    }
    CCLOG("COCOS AppDelegate gamelink On_StopRecord file=%s,voiceContent=%s,duration=%d",file.c_str(),voiceContent.c_str(),duration); //最长30s
    
    //to do
    _isRecord = false;
    gotyePlayer();
    eventGotye("GY_StooRecord");
    if (_isSend == false or duration < 1000) {
        return;
    }
    s_voiceContent = voiceContent;
    s_duration = duration;
    GameLink::uploadVoice(file);
}
void AppDelegate::onUploadVoice(const std::string& file, const std::string& url, const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_UploadVoice code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        return;
    }
    GLTarget user(GLTargetTypeChannel, s_group);
    GameLink::sendVoice(user, url, s_duration, s_group, s_voiceContent);
}
void AppDelegate::onSendMessage(const GLMessage& message, const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_SendMessage code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        return;
    }
    CCLOG("COCOS AppDelegate gamelink On_SendMessage sender= %s,duration=%d,data=%llu,id=%u",message.sender.account.c_str(),message.duration,message.date,message.id);
    if (_isPlay) {
        _gotyeMessage.push(message);
    }else{
        unsigned len = message.duration / 1000;
        noticeGotye(message.sender.account, len, message.id);
    }
}
void AppDelegate::onReceiveMessage(const gamelink::GLMessage& message)
{
    CCLOG("COCOS AppDelegate gamelink On_ReceiveMessage send:%s,duration:%d,id:%d ",message.sender.account.c_str(),message.duration,message.id);
    if(message.type == GLMessageTypeVoice){
        GameLink::downloadVoice(message);
    }
}
void AppDelegate::onDownloadVoice(const GLMessage& message, const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_DownloadVoice code = %d",error->code);
        return;
    }
    CCLOG("COCOS AppDelegate gamelink On_DownloadVoice send:%s,duration:%d,id:%d,file:%s ",message.sender.account.c_str(),message.duration,message.id,message.file.c_str());
    if(_isPlay || _isRecord){ // 如果正在播放声音 或 正在录制声音
        _gotyeMessage.push(message);
        return;
    }
    GameLink::playVoice(message);
}
void AppDelegate::onPlayStart(const GLMessage &message, const GLError* error) // 将要开始播放声音
{
    if (error) {
        CCLOG("COCOS AppDelegate gamelink On_PlayStart code = %d",error->code);
        return;
    }
    _isPlay = true;
    unsigned len = message.duration / 1000;
    noticeGotye(message.sender.account,len,message.id); // 通知自己播放who的声音
    eventGotye("GY_PlayStart");
    CCLOG("COCOS AppDelegate gamelink On_PlayStart send:%s,duration:%d,id:%d ",message.sender.account.c_str(),message.duration,message.id);
    
}
void AppDelegate::onPlayStop(const GLMessage &message)
{
    CCLOG("COCOS AppDelegate gamelink On_PlayStop send:%s,duration:%d,id:%d",message.sender.account.c_str(),message.duration,message.id);
    _isPlay = false;
    gotyePlayer();
}
void AppDelegate::onPlaying(const GLMessage &message, unsigned duration)
{
    CCLOG("AppDelegate gamelink onPlaying");
}

void AppDelegate::gotyePlayer()
{
    if (_gotyeMessage.size() > 0) {
        GLMessage msg = _gotyeMessage.front();
        _gotyeMessage.pop();
        if (msg.sender.account == _name){
            unsigned len = msg.duration / 1000;
            noticeGotye(_name, len, msg.id);
            return;
        }
        GameLink::playVoice(msg);
    }
}
void AppDelegate::noticeGotye(std::string name, unsigned len,unsigned id) //通知有新的语音消息
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    name.append(",");
    std::ostringstream tmp;
    tmp << len;
    name.append(tmp.str());
    name.append(",");
    tmp << id;
    name.append(tmp.str());
#else
    name.append(",");
    name.append(to_string(len));
    name.append(",");
    name.append(to_string(id));
#endif
    CCLOG("COCOS AppDelegate gotye_Notice sendToLua data= %s",name.c_str());
    _gotyeNotice->setData(name);
    _gotyeDispatcher->dispatchEvent(_gotyeNotice);
}
void AppDelegate::eventGotye(std::string data)
{
    _gotyeEvent->setData(data);
    _gotyeDispatcher->dispatchEvent(_gotyeEvent);
}

int gotyePlay(lua_State * l) //播放指定消息
{
    return 0;
}

void AppDelegate::onJoinChannel(const GLTarget& channel, const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_JoinChannel Fail code= %d", error->code);
        return;
    }
    s_group = channel.account;
    CCLOG("COCOS AppDelegate gamelink On_JoinChannel Succ group = %s",s_group.c_str());
    eventGotye(s_group);
}
void AppDelegate::onLeaveChannel(const GLTarget& room, const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_LeaveChannel code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        return;
    }
    s_group="";
    CCLOG("COCOS AppDelegate gamelink On_LeaveChannel account=%s",room.account.c_str());
}
int gotyeLogout(lua_State * l) // 退出
{
    GameLink::stopPlay();
    GameLink::logout();
    return 1;
}
void AppDelegate::onLogout(const GLError* error)
{
    if(error){
        CCLOG("COCOS AppDelegate gamelink On_Logout code=%d,detial=%s,description=%s",error->code,error->detail.c_str(),error->description.c_str());
        eventGotye("GY_Logout"+error->detail);
        return;
    }
    // 清空消息 & 还原状态
    _isPlay = false;
    _isRecord = false;
    while (!_gotyeMessage.empty()) {
        _gotyeMessage.pop();
    }
    CCLOG("COCOS AppDelegate gamelink On_Logout Success");
    eventGotye("GY_LogoutOk");
}
// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages(lua_State* L)
{
    luaopen_protobuf_c(L);
    luaopen_tea(L);
    luaopen_lsqlite3(L);
    lua_register(L, "PPgetLoc",PPgetLoc);                      // 高德定位
    lua_register(L, "PPSendWechatLogin", PPSendWechatLogin);   // 微信登录
    lua_register(L, "PPIsWechat", PPCheckWechat);              // 微信是否安装
    lua_register(L, "PPSendWechatImage", PPSendWechatImage);   // 微信分享图片
    lua_register(L, "PPSendWechatWeb", PPSendWechatWeb);       // 微信分享链接
    lua_register(L, "PPSendWechatPay", PPSendWechatPay);       // 微信支付
    lua_register(L, "PPIsGps", PPIsGps);                       // 定位是否开启
    lua_register(L, "PPCheckVoice", PPCheckVoice);             // 录音权限检查
    lua_register(L, "PPgetBattery", PPgetBattery);             // 电量获取
    lua_register(L, "PPDownloadFile", asyncFileTask);          // 文件下载
    lua_register(L, "PPUploadFile", PPUploadFile);             // 文件上传    这个有点问题 未解决 编译不过去
//    lua_register(L, "PPHttpRequest", PPHttpRequest);           // http 请求
//    lua_register(L, "PPTakePicture", GalleryCameraOpen);     // 照相机拍照
    lua_register(L, "PPMakeMD5", PPMakeMD5);                   // 获取MD5
    lua_register(L, "PPChoseImage", GalleryCameraChoose);      // 相册选取头像
    lua_register(L, "PPGetBytes", PPGetBytes);                 // 获取二进制
    lua_register(L, "PPMessageBox", PPMessageBox);             // 系统弹窗
    lua_register(L, "PPLuaLogger", PPLuaLogger);               // 日志文件
    lua_register(L,"PPCopyLua",CopyLua);                       // 复制到粘贴板
    lua_register(L,"PPOpenUrl",PPOpenUrl);                     // 打开网站
    lua_register(L,"PPSavePic",PPSavePic);                     // 保存图片到相册
    lua_register(L,"PPSetAlias",PPSetAlias);                   // 推送设置别名
    lua_register(L,"PPSetTags",PPSetTags);                     // 推送设置标签
    
    
//    lua_register(L, "gotyePlay",gotyePlay);     //播放指定语音消息 暂时废弃不用
    lua_register(L, "gotyeLogin",gotyeLogin);     //语音用户登录
    lua_register(L, "gotyeLogout", gotyeLogout);  //语音用户登出
    lua_register(L, "gotyeTalk", gotyeTalk);    //开始录音
    lua_register(L, "gotyeStop", gotyeStop);    //结束录音
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto fileUtils = FileUtils::getInstance();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto glview = director->getOpenGLView();
    glview->setDesignResolutionSize(1334, 750, ResolutionPolicy::EXACT_FIT);
#else
    
#endif
    auto wpath = fileUtils->getWritablePath();
    fileUtils->setPopupNotify(false);
//#if COCOS2D_DEBUG
    std::vector<std::string> files;
    fileUtils->listFilesRecursively(wpath, &files);
    int baseOffset = (int)wpath.length();
    for (auto path : files) {
        path = path.substr(baseOffset);
        CCLOG("COCOS AppDelegate path: %s",path.c_str());
    }
//#else
    bool result = GameLink::init("gled736341d2e27nag0753");
    CCLOG("COCOS AppDelegate gamelink init result:%d", result);
    GameLink::addObserver(*this);
    GameLink::enableLog(false);
    GameLink::clearCache();
    director->getScheduler()->schedule([](float tm){
        GameLink::pump();
    }, this, 0.05f, false,"gamelink");
//#endif
    // 更新逻辑 todo
    bool isUsers = fileUtils->isDirectoryExist(wpath+"users"); // 用户头像
    if (!isUsers){
        fileUtils->createDirectory(wpath+"users");
    }
    director->getEventDispatcher()->addCustomEventListener("NEED_RESTART_APP", [&](EventCustom* event){
        Director::getInstance()->purgeCachedData();
        SimpleAudioEngine::end();
        auto scene = UpdateScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });

    // set default FPS
    director->setAnimationInterval(1.0 / 60.0f);
    
    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);
    register_all_creator_reader_manual(L);
    register_all_packages(L);
    
    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));
    
    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());
    
#if CC_64BITS
    fileUtils->addSearchPath("src/64bit");
#endif
    fileUtils->addSearchPath(wpath+"update/");
    fileUtils->addSearchPath(wpath+"parent/");
    fileUtils->addSearchPath(wpath+"users/");
    fileUtils->addSearchPath("src");
    fileUtils->addSearchPath("res");
    
//    if (engine->executeScriptFile("main.lua"))
//    {
//        return false;
//    }
    auto scene = UpdateScene::createScene();
    Director::getInstance()->runWithScene(scene);
    return true;
}









