//
//  UpdateScene.hpp
//  TeamPoker
//
//  Created by ocean on 16/2/29.
//
//

#ifndef UpdateScene_hpp
#define UpdateScene_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

class UpdateScene : public cocos2d::Layer
{
private:
    cocos2d::Label* _progress;
    cocos2d::extension::AssetsManagerEx* _am;
    cocos2d::extension::EventListenerAssetsManagerEx* _amListener;
    cocos2d::Sequence* _seq;
    cocos2d::Sprite* _timerBg;
    cocos2d::ProgressTimer* _timer;
    cocos2d::Sprite* _state;
    cocos2d::Sprite* _bg;
    cocos2d::network::HttpRequest* _request;
    cocos2d::Label* _failTip;
    int _failtimes;
    std::string _version;
public:
    void onLoadEnd();
    void enterUpdate(cocos2d::Size wsize);
    void enterForce(std::string curVer);
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onExit() override;
    CREATE_FUNC(UpdateScene);
};

#endif /* UpdateScene_hpp */
