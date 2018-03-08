#include <time.h>
#include "Common.h"
#include "curl/curl.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "helper/Md5.h"
#include "helper/ToolsHelper.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
extern "C"{
#endif
	void qq_decrypt(uint32_t* crypt, uint32_t size, uint32_t* key, uint8_t** plain, uint32_t* p_size, uint32_t* fm);
	void qq_encrypt(uint8_t* plain, uint32_t size, uint32_t*key, uint32_t** crypt, uint32_t* c_size);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
}
#endif

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

static EventCustom asyncEvent = EventCustom("asyncTask");
static EventDispatcher* disEvent = Director::getInstance()->getEventDispatcher();
static AsyncTask* m_pInstance = nullptr;

AsyncTask::AsyncTask()
{
    _downloader = std::shared_ptr<network::Downloader>(new network::Downloader);
    
    _downloader->onFileTaskSuccess = [](const network::DownloadTask& task)
    {
        CCLOG("AsyncTask suc: %s,sto : %s,iden : %s",task.requestURL.c_str(),task.storagePath.c_str(),task.identifier.c_str());
        
        // 通知lua层
        asyncEvent.setData(task.identifier);
        disEvent->dispatchEvent(&asyncEvent);
    };
    _downloader->onTaskError = [](const network::DownloadTask& task,int errorCode,int errorCodeInternal,const std::string& errorStr)
    {
        CCLOG("AsyncTask err: %s,errorcode : %d,errorCodeInternal : %d,errorStr : %s",task.requestURL.c_str(),errorCode,errorCodeInternal,errorStr.c_str());
    };
    _wpath = FileUtils::getInstance()->getWritablePath();
//    _downloader->onTaskProgress = [this](const network::DownloadTask& task,int64_t bytesReceived,int64_t totalBytesReceived,int64_t totalBytesExpected)
//    {
//        CCLOG("pro url : %s,iden : %s,total : %lld,downed : %lld",task.requestURL.c_str(),task.identifier.c_str(),totalBytesExpected,totalBytesReceived);
//    };
    auto logPath = _wpath+"debug.log";
    _fileHanle = fopen(logPath.c_str(), "w");
}
AsyncTask* AsyncTask::getInstance()
{
    if (m_pInstance == nullptr){
        m_pInstance = new AsyncTask();
    }
    return m_pInstance;
}
void AsyncTask::addTask(const std::string &url, const std::string &sto,const std::string &iden)
{
    _downloader->createDownloadFileTask(url, _wpath+sto,iden);
}
void AsyncTask::fastFlush(const std::string &msg)
{
    char mbstr[100];
    time_t t = time(NULL);
    strftime(mbstr, 100, "%Y-%m-%d %H:%M:%S", localtime(&t));
    fprintf(_fileHanle, "[%s] %s\n",mbstr,msg.c_str());
    fflush(_fileHanle);
}
/*
 *  http 上传文件
 */
//上传文件的回调
size_t mywrite_data(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p)
{
    CCLOG("IN uploadFile::Finished");
    string szData = string((char*)dataBack); //返回的结果
//    CCLOG("IN uploadFile %s",szData.c_str());
    EventCustom event("uploadImage");
    event.setData(szData);
    disEvent->dispatchEvent(&event);
    return 0;
}
void UpLoadFileTmp(string url,string photoPath,string name,string token)
{
    CCLOG("IN uploadFile::Start");
    CURLcode cURLcode;
    CURL *cURL;
    
    //Init
    cURLcode = curl_global_init(CURL_GLOBAL_SSL);
    if (CURLE_OK != cURLcode)
    {
        curl_global_cleanup();
        return;
    }
    
    cURL = curl_easy_init();
    if (!cURL)
    {
        curl_easy_cleanup(cURL);
        curl_global_cleanup();
        return;
    }
    curl_httppost *post = NULL;
    curl_httppost *last = NULL;
    
    //上传的文件的全路径并指定类型
    curl_formadd(&post, &last, CURLFORM_COPYNAME, name.c_str(),
                 CURLFORM_FILE, photoPath.c_str(),
                 CURLFORM_CONTENTTYPE, "file", CURLFORM_END);//"Image/png"
    //设置多个参数
    curl_formadd(&post, &last,CURLFORM_COPYNAME,"fileName",
                CURLFORM_COPYCONTENTS,name.c_str(),CURLFORM_END);
    curl_formadd(&post, &last,CURLFORM_COPYNAME,"token",
                 CURLFORM_COPYCONTENTS,token.c_str(),CURLFORM_END);
    
    curl_easy_setopt(cURL, CURLOPT_URL, url.c_str());
    curl_easy_setopt(cURL, CURLOPT_TIMEOUT, 10);//超时时间10秒
    curl_easy_setopt(cURL, CURLOPT_HTTPPOST, post);//请求方式POST
    curl_easy_setopt(cURL, CURLOPT_WRITEFUNCTION, mywrite_data); //设置回调
    curl_easy_setopt(cURL, CURLOPT_VERBOSE, 0); //非零值表示你想CURL报告每一件意外的事情
    curl_easy_perform(cURL);
    
    curl_easy_cleanup(cURL);
    
    curl_global_cleanup();
    
}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

int PPTest(lua_State* l){
    
    //    auto director = Director::getInstance()->getEventDispatcher();
    //    std::string strData = "ocean";
    //    EventCustom event("HTTP_EVENT");
    //    event.setData(strData);
    //    director->dispatchEvent(&event);
    //    disEvent->dispatchEvent(&asyncEvent);
    //    ToolsHelper::createLocalMsg("title","content","data",1);
    ToolsHelper::checkVoice();
    return 1;
}

/*
 * 打上别名
 */


#endif

/*
 *  http 请求（加密）
 */
void onMyHttpRequestImageCompleted(HttpClient *sender, HttpResponse *response){
    
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    const char* tag = response->getHttpRequest()->getTag();
    
    if (0 != strlen(tag))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
    }
    
    if (!response->isSucceed())
    {
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    
    std::vector<char> *buffer = response->getResponseData();
    if (buffer->size() == 0)
    {
        return;
    }
    
    std::string strData;
    char* tmp = new char[buffer->size()+1];//[2046];
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        //char a = (*buffer)[i];
        //strData.append(1, a);
        tmp[i] = (*buffer)[i];
    }
    
    
    uint32_t key[4] = { 0x98325476, 0x9f42b57a, 0x9dc83b65, 0x6b9a5276 };
    //log(" buffer:%s", strData.c_str());
    uint8_t* plain;
    uint32_t p_size;
    uint32_t* fm=0;
    qq_decrypt((uint32_t*)tmp, (uint32_t)buffer->size(), key, &plain, &p_size,fm); // mode
    
    for (unsigned int i = 0; i < p_size; i++)
    {
        char a = plain[i];
        strData.append(1, a);
        
    }
    delete[] tmp;
    free(fm);
    
//    auto director = Director::getInstance()->getEventDispatcher();
    //char* buf = "{'cmd'='download_file','path'='d:/123.jpg'}";
    
    EventCustom event("HTTP_EVENT");
    event.setData(strData);
    disEvent->dispatchEvent(&asyncEvent);
//    director->dispatchEvent(&event);
}

/*
 * custom
 */
int PPgetLoc(lua_State* L){
    ToolsHelper::getGaoDeInfo();
    return 0;
}
int PPSendWechatLogin(lua_State* l){
    string scope  = (string)lua_tostring(l, 1);
    string state  = (string)lua_tostring(l, 2);
    ToolsHelper::sendLogin(scope,state);
    return 0;
}
int PPCheckWechat(lua_State* L){
    lua_pushboolean(L,ToolsHelper::checkWechat());
    return 1;
}
int PPSendWechatImage(lua_State* l)
{
    string icon = (string)lua_tostring(l, 1);
    int index = (int)lua_tointeger(l, 2) ? (int)lua_tointeger(l, 2) : 0 ;
    ToolsHelper::sendImageContent(icon, index); //0 会话 1 朋友圈
    return 0;
}
int PPSendWechatWeb(lua_State* l)
{
    string title = (string)lua_tostring(l, 1);
    string content = (string)lua_tostring(l, 2);
    string data = (string)lua_tostring(l, 3);
    int    scene = (int)lua_tonumber(l, 4);
    ToolsHelper::sendLinkContent(title,content,data,scene);
    return 0;
}
int PPSendWechatApp(lua_State* l)
{
    string title = (string)lua_tostring(l, 1);
    string content = (string)lua_tostring(l, 2);
    string data = (string)lua_tostring(l, 3);
    string icon = (string)lua_tostring(l, 4);
    ToolsHelper::sendAppContent(title, content, data,icon);
    return 0;
}
int PPSendWechatLet(lua_State* l)
{
    string content = (string)lua_tostring(l, 1);
    ToolsHelper::sendTextContent(content);
    return 0;
}
int PPSendWechatPay(lua_State* l)
{
    string partnerId = (string)lua_tostring(l, 1);
    string prepayId = (string)lua_tostring(l, 2);
    string nonceStr = (string)lua_tostring(l, 3);
    string timeStamp = (string)lua_tostring(l, 4);
    string sign = (string)lua_tostring(l, 5);
    ToolsHelper::sendWechatPay(partnerId, prepayId, nonceStr, timeStamp, sign);
    return 0;
}
int PPIsGps(lua_State *L){
    bool isg = ToolsHelper::checkGps();
    int gps = 0;
    if(isg){
        gps = 1;
    }
    lua_pushinteger(L,gps);
    return 1;
}
int PPCheckVoice(lua_State* L){
    ToolsHelper::checkVoice();
    return 0;
}
int PPgetBattery(lua_State *L){
    lua_pushinteger(L, ToolsHelper::getBattery());
    return 1;
}
int asyncFileTask(lua_State* L){
    string url = (string)lua_tostring(L, 1); // url 下载地址
    string sto = (string)lua_tostring(L, 2); // 储存地址，（标志与他等同）："users/a.JPG"
    string iden = (string)lua_tostring(L, 3);
    AsyncTask::getInstance()->addTask(url, sto,iden);
    return 1;
}
int PPUploadFile(lua_State * l){
    string url = (string)lua_tostring(l, 1);
    string filename = (string)lua_tostring(l, 2);
    string name = (string)lua_tostring(l, 3);
    string token= (string)lua_tostring(l, 4);
    UpLoadFileTmp(url, filename, name,token);
    return 1;
}
int PPHttpRequest(lua_State * l){
    string url = (string)lua_tostring(l, 1);
    string requestData = (string)lua_tostring(l, 2);
    string tag = (string)lua_tostring(l, 3);
    
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    
    //这是回调对象和回调函数
    request->setResponseCallback(&onMyHttpRequestImageCompleted);
    request->setTag(tag.c_str());
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    
    //请求的数据
    uint32_t* crypt=0;
    if (strlen(requestData.c_str()) != 0){
        uint8_t* plain = (uint8_t*)requestData.c_str();
        uint32_t p_size = (uint32_t)strlen(requestData.c_str()); // mode
        
        uint32_t c_size;
        uint32_t key[4] = { 0x98325476, 0x9f42b57a, 0x9dc83b65, 0x6b9a5276 };
        qq_encrypt(plain, p_size, key, &crypt, &c_size);
        //request->setRequestData(requestData.c_str(), strlen(requestData.c_str()));
        request->setRequestData((char*)crypt, c_size);
    }
    HttpClient::getInstance()->enableCookies(NULL);
    HttpClient::getInstance()->send(request);
    
    //释放内存
    request->release();
    free(crypt);
    return 1;
}
int PPMakeMD5(lua_State * l){
    string text = (string)lua_tostring(l, 1);
    MD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)text.c_str(), (int)strlen(text.c_str())); // mode
    string result = iMD5.ToString();
    lua_pushstring(l, result.c_str());
    return 1;
}
int GalleryCameraChoose(lua_State* L){
    string uid = (string)lua_tostring(L, 1);
    ToolsHelper::callOpenImageChoose(uid);
    return 1;
}
int GalleryCameraOpen(lua_State* L){
    ToolsHelper::callOpenCamera();
    return 1;
}
int PPGetBytes(lua_State* L){
    auto file = FileUtils::getInstance();
    string path = (string)lua_tostring(L, 1);
    path = file->fullPathForFilename(path);
    Data data = file->getDataFromFile(path);
    lua_pushlstring(L, (const char*)data.getBytes(), data.getSize());
    return 1;
}
int PPMessageBox(lua_State* L){
#if (COCOS2D_DEBUG > 0)
    MessageBox(lua_tostring(L, 1),lua_tostring(L, 2));
#endif
    return 1;
}
int PPLuaLogger(lua_State* L){
    string msg = (string)lua_tostring(L, 1);
    AsyncTask::getInstance()->fastFlush(msg);
    return 0;
}
int CopyLua(lua_State *l){
    string text  = (string)lua_tostring(l, 1);
    ToolsHelper::copy(text);
    return 0;
}
int PPOpenUrl(lua_State* l){
    string url =(string)lua_tostring(l, 1);
    Application::getInstance()->openURL(url);
    return 0;
}
int PPSetAlias(lua_State* l){
    string alias = (string)lua_tostring(l, 1);
    ToolsHelper::setAlias(alias);
    return 0;
}
int PPSetTags(lua_State* l){
    string tags = (string)lua_tostring(l, 1);
    ToolsHelper::setTags(tags);
    return 0;
}
int PPDelTags(lua_State* L){
    ToolsHelper::delTags();
    return 0;
}
int PPSavePic(lua_State* L){
    string path = (string)lua_tostring(L, 1);
    ToolsHelper::saveImage(path);
    return 0;
}
