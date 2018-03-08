//
//  UpdateScene.cpp
//  TeamPoker
//
//  Created by ocean on 16/2/29.
//
//

#include <sstream>

#include "UpdateScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "./cocos/scripting/lua-bindings/manual/CCLuaEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocos2d::network;

Scene* UpdateScene::createScene()
{
    auto scene = Scene::create();
    auto layer = UpdateScene::create();
    scene->addChild(layer);
    return scene;
}
bool UpdateScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto wsize = Director::getInstance()->getVisibleSize();
    float bw = 1334;
    float bh = 750;
    _failtimes = 0;
    //bg
    _bg = Sprite::create("creator/Texture/login/loginbg.png");
    _bg->setPosition(Vec2(wsize.width/2,wsize.height/2));
    _bg->setScale(wsize.width/bw, wsize.height/bh);
    this->addChild(_bg);
    
//    auto spTitle = Sprite::create("creator/loginTitle.png"); // title
//    spTitle->setPosition(Vec2(bw/2,bh-200));
//    _bg->addChild(spTitle);
    
    //progresstimer
//    _timerBg = Sprite::create("creator/progressBg.png");
//    _timerBg->setPosition(Vec2(bw/2,70));
//    _bg->addChild(_timerBg);
//    _timer = ProgressTimer::create(Sprite::create("creator/progress.png"));
//    _timer->setBarChangeRate(Vec2(1,0));
//    _timer->setType(ProgressTimer::Type::BAR);
//    _timer->setMidpoint(Vec2(0,1));
//    _timer->setPosition(Vec2(bw/2,70));
//    _timer->setPercentage(0.0f);
//    _bg->addChild(_timer);
    
    // label progress
    _progress = Label::createWithSystemFont("加载中 0%", "Arial",30); //912*6
    _progress->setAnchorPoint(Vec2(0,0.5));
    _progress->setPosition(Vec2(bw/2-80,70));
    _progress->setColor(Color3B(255,0,0 ));
    _bg->addChild(_progress);
    
    _failTip = Label::createWithSystemFont("无网络链接", "Arial",70);
    _failTip->setPosition(Vec2(bw/2,bh/2-80));
    _failTip->setColor(Color3B::RED);
    _failTip->setVisible(false);
    _failTip->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.6, 0.8),ScaleTo::create(0.6, 1.2))));
    _bg->addChild(_failTip);
    
    _version = "0.0.0";
    enterUpdate(wsize); // hot update
//#if COCOS2D_DEBUG
//    this->runAction(_seq); //test //     _timer->setPercentage(100);
//#else
//    _timerBg->setVisible(false);
//    _timer->setVisible(false);
    _progress->setVisible(false);
    enterForce(_version); // force update
//#endif
    return true;
}
void UpdateScene::enterForce(std::string curVer)
{
    // 检查强制更新问题
    std::string checkVer = "http://101.201.198.69:50007/platform/pass/app/update/status"; // 没办法为了兼容以前 123.57.62.251
//    curVer = "0.-1.32";
    curVer = StringUtils::format("{\"gameId\":0,\"version\":\"%s\"}",curVer.c_str());
    _request = new (std::nothrow) HttpRequest();
    _request->setUrl(checkVer);
    _request->setRequestType(HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    _request->setHeaders(headers);
    _request->setResponseCallback([&](HttpClient *sender, HttpResponse *response){
        if (!response)
        {
            return;
        }
        if (0 != strlen(response->getHttpRequest()->getTag()))
        {
            CCLOG("COCOS UpdateScene ForceUpdate %s completed", response->getHttpRequest()->getTag());
        }
        long statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
        CCLOG("COCOS UpdateScene ForceUpdate response code: %ld", statusCode);
        if (!response->isSucceed())
        {
            if (_failtimes > 20){
                _failTip->setVisible(true);
            }
            CCLOG("COCOS UpdateScene ForceUpdate error=%s, failtimes=%d",response->getErrorBuffer(),_failtimes);
            HttpClient::getInstance()->send(_request);
            _failtimes = _failtimes + 1;
            return;
        }
        _failTip->setVisible(false);
        _failTip->stopAllActions();
        std::vector<char> *buffer = response->getResponseData();
        std::string res;
        rapidjson::Document data;
        res.insert(res.begin(), buffer->begin(), buffer->end());
        data.Parse<0>(res.c_str());
        CCLOG("COCOS UpdateScene ForceUpdate response = %s",res.c_str());
        
        if (data.HasParseError()){
            CCLOG("COCOS UpdateScene ForceUpdate GetParseError %u\n",data.GetParseError());
        }
        if (data.IsObject() && data.HasMember("data")){
            int status = data["data"]["status"].GetInt();
//            status = 2;
            CCLOG("COCOS UpdateScene ForceUpdate status: %d",status);// 1强制更新 2在线更新 3最新状态
            if (status == 1){
                std::string msg = "发现新版本，需要重新下载";
                if (data["data"].HasMember("msg")){ msg = data["data"]["msg"].GetString(); }
                std::string url = "https://www.baidu.com";
                if (data["data"].HasMember("updateUrl")){ url = data["data"]["updateUrl"].GetString(); }
                auto sp = Sprite::create("ui/popcom1.png");
                sp->setPosition(Vec2(667,375));
                _bg->addChild(sp,100);
                Button* btnCancle = Button::create("ui/btncom2.png","ui/btncom2.png");
                auto pic = Sprite::create("ui/btntext2.png");
                pic->setPosition(Vec2(83,32));
                btnCancle->addChild(pic);
                btnCancle->setPosition(Vec2(285-120,60));
                btnCancle->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
                   if (type == Widget::TouchEventType::ENDED) {
                       _bg->removeAllChildrenWithCleanup(true);
                   }
                });
                sp->addChild(btnCancle);
                Button* btnConfirm = Button::create("ui/btncom1.png","ui/btncom1.png");
                auto pic2 = Sprite::create("ui/btntext1.png");
                pic2->setPosition(Vec2(83,32));
                btnConfirm->addChild(pic2);
                btnConfirm->setPosition(Vec2(285+120,60));
                btnConfirm->addTouchEventListener([&,url](Ref*, Widget::TouchEventType type){
                     if (type == Widget::TouchEventType::ENDED) {
                         FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath()+"update");
                         Application::getInstance()->openURL(url);
                     }
                });
                sp->addChild(btnConfirm);
                auto lab = Label::createWithSystemFont(msg, "Arial",36);
                lab->setColor(Color3B::BLACK);
                lab->setPosition(Vec2(285,205));
                sp->addChild(lab);
            }else if (status == 2){
//                _timer->setVisible(true);
//                _timerBg->setVisible(true);
                _progress->setVisible(true);
                this->runAction(_seq);
            }else if(status == 3){
                this->onLoadEnd();
            }
        }
        if (response->getHttpRequest()->getReferenceCount() != 2)
        {
            CCLOG("COCOS UpdateScene ForceUpdate request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
        }
        
    });
    _request->setTag("ForceUpdate");
    const char* postData = curVer.c_str();
    _request->setRequestData(postData, strlen(postData));
    HttpClient::getInstance()->send(_request);
}
void UpdateScene::enterUpdate(Size wsize)
{
    // update logic
    std::string mainfestPath = "project.manifest";
    std::string storagePath = FileUtils::getInstance()->getWritablePath()+"update/";
    _am = AssetsManagerEx::create(mainfestPath, storagePath);
    _am->retain();
    
    // show version
    _version = _am->getLocalManifest()->getVersion();
    CCLOG("COCOS UpdateScene mainfest: %s",_version.c_str());
    UserDefault::getInstance()->setStringForKey("gameId0", _version.c_str());
    auto version = Label::createWithSystemFont("","Arial",30);
    version->setPosition(Vec2(wsize.width-50,20));
    version->setColor(Color3B::RED);
    addChild(version);
    version->setString(_version.c_str());
    
    // execute update
    _seq = Sequence::create(CallFunc::create([&]{ //DelayTime::create(0.1),
        if (!_am->getLocalManifest()->isLoaded()) {
            CCLOG("COCOS UpdateScene Fail to update assets,step skipped.");
            onLoadEnd();
        }else{
            _amListener = cocos2d::extension::EventListenerAssetsManagerEx::create(_am, [this](EventAssetsManagerEx* event){
                static int failCount = 0;
                switch (event->getEventCode()) {
                    case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                    {
                        CCLOG("COCOS UpdateScene No local mainfest file found,skip assets update.");
                        this->onLoadEnd();
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                    {
//                        CCLOG("UpdateScene total->%s",event->getMessage().c_str());
                        std::string assetId = event->getAssetId();
                        float percent = event->getPercent();
                        std::string str;
                        if (assetId == AssetsManagerEx::VERSION_ID) {
                            str = StringUtils::format("Version file: %.2f",percent) + "%";
                        }else if (assetId == AssetsManagerEx::MANIFEST_ID){
                            str = StringUtils::format("Mainfest file: %.2f",percent) + "%";
                        }else{
                            percent = event->getPercentByFile();
                            str = StringUtils::format("%.2f",percent) + "%";
                            _progress->setString(str);
//                            _timer->setPercentage(percent);
                        }
                        CCLOG("COCOS UpdateScene ----->>>> %s ----->>,%f",str.c_str(),percent);
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                    case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                    {
                        CCLOG("COCOS UpdateScene Fail to download mainfest file, update skipped");
                        this->onLoadEnd();
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                    {
                        CCLOG("COCOS UpdateScene HotUpdate already_up_to_date");
                        this->onLoadEnd();
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                    {
                        std::string upver = event->getMessage();
                        CCLOG("COCOS UpdateScene HotUpdate finished: %s",upver.c_str());
                        UserDefault::getInstance()->setStringForKey("gameId0",upver.c_str());
                        this->onLoadEnd();
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                    {
                        CCLOG("COCOS UpdateScene Update failed. %s",event->getMessage().c_str());
                        failCount++;
                        if (failCount < 5) {
                            _am->downloadFailedAssets();
                        }else{
                            CCLOG("COCOS UpdateScene Reach maximum fail cout,exit update progeress");
                            failCount = 0;
                            this->onLoadEnd();
                        }
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                    {
                        CCLOG("COCOS UpdateScene Asset %s : %s",event->getAssetId().c_str(),event->getMessage().c_str());
                    }
                        break;
                    case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                    {
                        CCLOG("COCOS UpdateScene error decopress %s",event->getMessage().c_str());
                    }
                        break;
                    default:
                        break;
                }
            });
            Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_amListener, 1);
            _am->update();
        }
        
    }), NULL);
    _seq->retain();
}
void UpdateScene::onLoadEnd()
{
    CCLOG("COCOS UpdateScene HotUpdate on_LoadEnd");
//    _timer->setPercentage(100);
    _progress->setString("加载完成 100%");
    FileUtils::getInstance()->purgeCachedEntries();
    this->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create([&](){
        LuaEngine::getInstance()->executeScriptFile("main.lua");
    }), nullptr));
}

void UpdateScene::onExit()
{
    CCLOG("COCOS UpdateScene HotUpdate onExit");
    Layer::onExit();
    UserDefault::getInstance()->flush();
    _eventDispatcher->removeEventListener(_amListener);
    _am->release();
    _seq->release();
//#if COCOS2D_DEBUG
//#else
    _request->release();
//#endif
}
