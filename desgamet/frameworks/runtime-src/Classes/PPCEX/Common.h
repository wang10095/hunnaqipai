#include "base/ccConfig.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif
#include <iostream>
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::network;

class AsyncTask
{
private:
    AsyncTask();
    std::shared_ptr<network::Downloader> _downloader;
    std::string _wpath;
    FILE* _fileHanle;
public:
    static AsyncTask* getInstance();
    void addTask(const std::string& url,const std::string &sto,const std::string &iden);
    void fastFlush(const std::string &msg);
};

int PPTest(lua_State *l);
/*
 * custom
 */
int PPgetLoc(lua_State* L);            // 高德定位
int PPSendWechatLogin(lua_State* L);   // 微信登录
int PPCheckWechat(lua_State* L);       // 微信是否安装  测试不准确
int PPSendWechatImage(lua_State* L);   // 微信分享图片
int PPSendWechatWeb(lua_State* L);     // 微信网页链接
int PPSendWechatApp(lua_State* L);     // 微信app链接  废弃
int PPSendWechatLet(lua_State* L);     // 微信文字     废弃
int PPSendWechatPay(lua_State* L);     // 微信支付     未测试
int PPIsGps(lua_State *L);             // 定位是否开启
int PPCheckVoice(lua_State *L);        // 检查录音权限
int PPgetBattery(lua_State *L);        // 检查电量
int asyncFileTask(lua_State* L);       // 下载文件
int PPUploadFile(lua_State * l);       // 上传文件
int PPHttpRequest(lua_State * l);      // http 请求 暂未使用
int PPMakeMD5(lua_State * l);          // 获取md5 暂未使用
int GalleryCameraChoose(lua_State* L); // 从相册选取头像
int GalleryCameraOpen(lua_State* L);   // 从照相机拍照
int PPGetBytes(lua_State* l);          // 获取二进制
int PPMessageBox(lua_State* L);        // 系统弹窗
int PPLuaLogger(lua_State *L);         // 日志文件
int CopyLua(lua_State *L);             // 复制文本
int PPOpenUrl(lua_State *L);           // 打开网址
int PPSetAlias(lua_State* L);          // 给设备打上别名
int PPSetTags(lua_State* L);           // 给设备打上标签
int PPDelTags(lua_State* L);           // 删除设备标签
int PPSavePic(lua_State* L);           // 保存图片到相册
