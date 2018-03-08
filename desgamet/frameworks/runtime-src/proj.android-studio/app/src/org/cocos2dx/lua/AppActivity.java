/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2016 cocos2d-x.org
Copyright (c) 2013-2017 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.lua;

import android.annotation.SuppressLint;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;
import com.nabai.desgame.wxapi.WXEntryActivity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;

import android.os.Environment;
import android.os.Looper;
import android.provider.MediaStore;
import android.util.Log;

import java.net.URISyntaxException;
import java.util.Map;
import java.util.Set;
import java.util.LinkedHashSet;
import android.text.TextUtils;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.UnsupportedEncodingException;
import java.io.IOException;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import android.os.Handler;
import android.content.Intent;
import com.tencent.mm.opensdk.modelpay.PayReq;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import com.tencent.mm.opensdk.modelmsg.WXImageObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXTextObject;
import com.tencent.mm.opensdk.modelmsg.WXWebpageObject;
import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationClientOption;
import com.amap.api.location.AMapLocationClientOption.AMapLocationMode;
import com.amap.api.location.AMapLocationListener;
import com.amap.api.location.AMapLocationQualityReport;
import cn.jpush.android.api.JPushInterface;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.os.PowerManager;
import android.content.Context;
import android.location.LocationManager;

public class AppActivity extends Cocos2dxActivity{
    public static final String APP_ID = "wx0fd58048862fa7dc";
    public static final String WX_APP_SECRET = "5eac194c81a4d349c7e07e96c5acf3fb";
    public static IWXAPI wxApi;
    public static String code;
    private static AppActivity instance;
    public static AMapLocationClient locationClient = null;
    public static int curBatter;
    private BroadcastReceiver batteryLevelRcvr; //battery
    private IntentFilter batteryLevelFilter;
    private PowerManager.WakeLock mWakeLock;
    private static final int REQUEST_CODE_GETIMAGE_BYSDCARD = 0;// 从sd卡得到图像的请求码 // images
    private static final int REQUEST_CODE_CROP_PICTURE = 4;
    private static String CROP_FILE_PATH;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }

        // DO OTHER INITIALIZATION BELOW
        instance = this;
        regBatter();
        regToWx();
        regGaode();
        regJpush();
        PowerManager powerManager= (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "keep_screen_on_tag");

        new Handler().postDelayed(new Runnable() {
            public void run() {
                Intent in = getIntent();
                String s1 = in.toURI();
                String s2 = in.getDataString();

                Log.d("COCOS", "--> S1 Handler URI: "+s1);
                if (s2 != null) {
                    Log.d("COCOS", "-- s2 Handler URL: "+s2);
                    String[] arr = s2.split("[?]");
                    String query = arr[1];
                    Log.d("COCOS","-zz- s2 Handler URL: "+query);
                    JniCallCocos.StartWithUrlCallback(query,"OUT_START_APP");
                } else {
                    s2 = in.getStringExtra("URL");
                    if (s2 != null){
                        Log.d("COCOS","--> S3 Handler URL: "+s2);
                        finish();
                        // JniCallCocos.StartWithUrlCallback(s2);
                    }
                }
                // debug
//                sendWechatLogin("snsapi_userinfo,snsapi_base","ndesgame");
//                getGaodeLoc();
//                openImageChoose();
//                sendLinkContent("hello","world","http://www.baidu.com",1);
            }
        }, 2000);
    }
    private void regToWx(){
        wxApi = WXAPIFactory.createWXAPI(this, APP_ID, true);
        wxApi.registerApp(APP_ID);
    }
    public static void WechatGetToken(String token, String openid,String refresh_token) {
        Log.d("COCOS","AppActivity WechatGetToken->"+token);
        String str = token+","+openid+","+refresh_token;
        JniCallCocos.WxLoginCallback(str);
    }
    private void WXGetAccessToken() {
        if (code == null)
            return;

        HttpClient get_access_token_httpClient = new DefaultHttpClient();
        String access_token = "";
        String openid = "";
        String refresh_token = "";
        try {
            String url = WXEntryActivity.getCodeRequest(code);
            // Log.d("url", url);
            HttpPost postMethod = new HttpPost(url);
            HttpResponse response = get_access_token_httpClient
                    .execute(postMethod); // 执行POST方法
            if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                InputStream is = response.getEntity().getContent();
                BufferedReader br = new BufferedReader(
                        new InputStreamReader(is));
                String str = "";
                StringBuffer sb = new StringBuffer();
                while ((str = br.readLine()) != null) {
                    sb.append(str);
                }
                is.close();
                String josn = sb.toString();
                Log.d("COCOS","WXGetAccessToken post="+josn);
                JSONObject json1 = new JSONObject(josn);
                if (json1.has("access_token")) {
                    access_token = (String) json1.get("access_token");
                    openid = (String) json1.get("openid");
                    refresh_token = (String) json1.get("refresh_token");
                    // 测试弹出获取到的token
                    AppActivity.WechatGetToken(access_token, openid,refresh_token);
                }
            } else {
            }
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
        code = null;
    }
    private class AccessTokenThread extends Thread {
        @Override
        public void run() {
            WXGetAccessToken();
        }
    }
    // 微信登录
    public static void sendWechatLogin(String scope, String state){
        Log.d("COCOS","AppActivity: sendWechatLogin -> scope="+scope+" state="+state);
        SendAuth.Req request = new SendAuth.Req();
        request.scope    = scope;
        request.state    = state;
        wxApi.sendReq(request);
    }
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        String s1 = intent.toURI();
        String s2 = intent.getDataString();
        Log.d("COCOS", "onNewIntent URI: "+s1);
        if (s2 != null) {
            Log.d("COCOS", "onNewIntent -> URL: "+s2);
            String[] arr = s2.split("[?]");
            String query = arr[1];
            JniCallCocos.StartWithUrlCallback(query,"OUT_START_APP");
        }
    }
    // 微信支付
    public static void sendWechatPay(String partnerId, String prepayId,
                                     String nonceStr, String timeStamp, String sign){
        Log.d("COCOS","AppActivity:sendWechatPay ->partnerId="+partnerId+" prepayId="+prepayId);
        PayReq request = new PayReq();
        request.appId = APP_ID;
        request.partnerId = partnerId;
        request.prepayId = prepayId;
        request.packageValue = "Sign=WXPay";
        request.nonceStr = nonceStr;
        request.timeStamp = timeStamp;
        request.sign = sign;
        wxApi.sendReq(request);
    }
    private static String buildTransaction(final String type) {
        return (type == null) ? String.valueOf(System.currentTimeMillis())
                : type + System.currentTimeMillis();
    }
    // 微信发送图片
    public static void sendWechatImage(String file, int scene){
        Log.d("COCOS","AppActivity: sendWechatImage --> file="+file+" scene="+scene);
        if (wxApi.openWXApp()){
            Bitmap bmp = null;
            InputStream is = null;
            try{
                // is = instance.getResources().getAssets().open(file);
                bmp = BitmapFactory.decodeFile(file);
            }catch(Exception e){
                Log.d("COCOS","sendImageContent open error");
            }finally{
                try{
                    if (is !=  null) is.close();
                }catch(Exception e){
                    Log.d("COCOS","sendImageContent close error");
                }
            }
            // bmp = createBitmapThumbnail(bmp);

            WXImageObject imgObj = new WXImageObject(bmp);
            // imgObj.setImagePath(file);
            WXMediaMessage msg = new WXMediaMessage();
            Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, 50, 100, true);
            bmp.recycle();
            msg.thumbData = Util.bmpToByteArray(thumbBmp, true);  // 设置缩略图
            msg.mediaObject = imgObj;

            SendMessageToWX.Req req = new SendMessageToWX.Req();
            req.transaction = buildTransaction("img");
            req.message = msg;
            // req.scene = SendMessageToWX.Req.WXSceneTimeline;
            req.scene = scene;
            wxApi.sendReq(req);
        }
    }
    // 微信发送链接
    public static void sendLinkContent(String title,String content,String url, int scene){
        Log.d("COCOS","AppActivity: sendLinkContent -->  url="+url+" scene="+scene+"content="+content);
        if (wxApi.openWXApp()){
            WXWebpageObject webpage = new WXWebpageObject();
            webpage.webpageUrl = url;
            WXMediaMessage msg = new WXMediaMessage(webpage);
            msg.title = title;
            msg.description = content;
            // Bitmap thumb = BitmapFactory.decodeResource(instance.getResources(), R.drawable.icon);
            // msg.thumbData = Util.bmpToByteArray(thumb, true);
            SendMessageToWX.Req req = new SendMessageToWX.Req();
            req.transaction = buildTransaction("webpage");
            req.message = msg;
            req.scene = scene;//SendMessageToWX.Req.WXSceneSession;
            wxApi.sendReq(req);
        }
    }
    public static void sendTextContent(String content){
        Log.d("cocos ->AppActivity::sendTextContent","content="+content);
        if (wxApi.openWXApp()){
            WXTextObject textObj = new WXTextObject();
            textObj.text = content;
            WXMediaMessage msg = new WXMediaMessage();
            msg.mediaObject = textObj;
            msg.description = content;
            SendMessageToWX.Req req = new SendMessageToWX.Req();
            req.transaction = buildTransaction("text");
            req.message = msg;
            req.scene = SendMessageToWX.Req.WXSceneSession;
            wxApi.sendReq(req);
        }
    }
    @Override
    protected void onResume() {
        super.onResume();
        mWakeLock.acquire();
        Log.d("COCOS", "AppActivity: onResume");
        if (code != null) {
            new AccessTokenThread().start();
        }
    }
    @Override
    protected void onPause() {
        super.onPause();
    }
    @Override
    protected void onStop() {
        super.onStop();
        mWakeLock.release();
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        destroyLocation();
        unregisterReceiver(batteryLevelRcvr);
    }
    //高德
    public static AMapLocationClientOption getDefaultOption(){
        AMapLocationClientOption mOption = new AMapLocationClientOption();
        mOption.setLocationMode(AMapLocationMode.Hight_Accuracy);//可选，设置定位模式，可选的模式有高精度、仅设备、仅网络。默认为高精度模式
        mOption.setGpsFirst(true);//可选，设置是否gps优先，只在高精度模式下有效。默认关闭
        mOption.setHttpTimeOut(30000);//可选，设置网络请求超时时间。默认为30秒。在仅设备模式下无效
        mOption.setInterval(2000);//可选，设置定位间隔。默认为2秒
        mOption.setNeedAddress(true);//可选，设置是否返回逆地理地址信息。默认是true
        mOption.setOnceLocation(true);//可选，设置是否单次定位。默认是false
        mOption.setOnceLocationLatest(false);//可选，设置是否等待wifi刷新，默认为false.如果设置为true,会自动变为单次定位，持续定位时不要使用
        mOption.setLocationCacheEnable(true); //可选，设置是否使用缓存定位，默认为true
        return mOption;
    }
    private String getGPSStatusString(int statusCode){
        String str = "";
        switch (statusCode){
            case AMapLocationQualityReport.GPS_STATUS_OK:
                str = "GPS状态正常";
                break;
            case AMapLocationQualityReport.GPS_STATUS_NOGPSPROVIDER:
                str = "手机中没有GPS Provider，无法进行GPS定位";
                break;
            case AMapLocationQualityReport.GPS_STATUS_OFF:
                str = "GPS关闭，建议开启GPS，提高定位质量";
                break;
            case AMapLocationQualityReport.GPS_STATUS_MODE_SAVING:
                str = "选择的定位模式中不包含GPS定位，建议选择包含GPS定位的模式，提高定位质量";
                break;
            case AMapLocationQualityReport.GPS_STATUS_NOGPSPERMISSION:
                str = "没有GPS定位权限，建议开启gps定位权限";
                break;
        }
        return str;
    }
    public synchronized static String getLocationStr(AMapLocation location){
        if(null == location){
            return null;
        }
        StringBuffer sb = new StringBuffer();
        Log.d("COCOS","COCOS GAODE getLocationStr->code:"+location.getErrorCode());
        if(location.getErrorCode() == 0){
            String str = location.getLongitude()+","+location.getLatitude()+","+location.getCity()+","+location.getDistrict();
            Log.d("COCOS","COCOS getLongitude->"+str);
            JniCallCocos.GaoDeCallback(str);
            sb.append("定位成功" + "\n");
            sb.append("定位类型: " + location.getLocationType() + "\n");
            sb.append("经    度    : " + location.getLongitude() + "\n");
            sb.append("纬    度    : " + location.getLatitude() + "\n");
            sb.append("精    度    : " + location.getAccuracy() + "米" + "\n");
            sb.append("提供者    : " + location.getProvider() + "\n");
            sb.append("速    度    : " + location.getSpeed() + "米/秒" + "\n");
            sb.append("角    度    : " + location.getBearing() + "\n");
            // 获取当前提供定位服务的卫星个数
            sb.append("星    数    : " + location.getSatellites() + "\n");
            sb.append("国    家    : " + location.getCountry() + "\n");
            sb.append("省            : " + location.getProvince() + "\n");
            sb.append("市            : " + location.getCity() + "\n");
            sb.append("城市编码 : " + location.getCityCode() + "\n");
            sb.append("区            : " + location.getDistrict() + "\n");
            sb.append("区域 码   : " + location.getAdCode() + "\n");
            sb.append("地    址    : " + location.getAddress() + "\n");
            sb.append("兴趣点    : " + location.getPoiName() + "\n");
            sb.append("定位时间: " + Util.formatUTC(location.getTime(), "yyyy-MM-dd HH:mm:ss") + "\n");
            sb.append("***定位质量报告***").append("\n");
            sb.append("* WIFI开关：").append(location.getLocationQualityReport().isWifiAble() ? "开启":"关闭").append("\n");
            sb.append("* GPS状态：").append(instance.getGPSStatusString(location.getLocationQualityReport().getGPSStatus())).append("\n");
            sb.append("* GPS星数：").append(location.getLocationQualityReport().getGPSSatellites()).append("\n");
            sb.append("回调时间: " + Util.formatUTC(System.currentTimeMillis(), "yyyy-MM-dd HH:mm:ss") + "\n");
        } else {
            //定位失败
            String str = "0,0,,";
            JniCallCocos.GaoDeCallback(str);
        }
        return sb.toString();
    }
    public static void regGaode(){
        AMapLocationListener locationListener = new AMapLocationListener() {
            // @Override
            public  void onLocationChanged(AMapLocation loc) {
                if (null != loc) {
                    //解析定位结果
                    String result = getLocationStr(loc);
                    Log.d("COCOS","COCOS GAODE -> success，location is" + result);
                } else {
                    String str = "0,0,,";
                    JniCallCocos.GaoDeCallback(str);
                    Log.d("COCOS","COCOS GAODE -> fail location is null");
                }
            }
        };
        locationClient = new AMapLocationClient(instance.getApplicationContext());
        locationClient.setLocationOption(getDefaultOption());
        locationClient.setLocationListener(locationListener);
    }
    // 高德定位
    public static void getGaodeLoc(){
        Log.d("COCOS","COCOS GAODE getGaodeLoc");
        locationClient.startLocation();
    }
    private void destroyLocation(){
        if (null != locationClient) {
            locationClient.onDestroy();
            locationClient = null;
        }
    }
    // 推送
    private void regJpush(){
        JPushInterface.setDebugMode(true);
        JPushInterface.init(this);
    }
    public static void setAlias(String alias){
        Log.d("COCOS","COCOS setAlias:"+alias);
        JPushInterface.setAlias(instance,0,alias);
    }
    public static void setTags(String tags){
        if (TextUtils.isEmpty(tags)){
            return;
        }
        String[] sArray = tags.split(",");
        Set<String> tagSet = new LinkedHashSet<String>();
        for (String sTagItme : sArray) {
            tagSet.add(sTagItme);
        }
        JPushInterface.setTags(instance,1,tagSet);
    }
    public static void delTags(){
        JPushInterface.cleanTags(instance,2);
    }
    //电量
    public void regBatter(){
        curBatter = 100;
        batteryLevelRcvr = new BroadcastReceiver() {
            public void onReceive(Context context, Intent intent) {
                int current = intent.getExtras().getInt("level");// 获得当前电量
                int total = intent.getExtras().getInt("scale");// 获得总电量
                int percent = current * 100 / total;
                curBatter = percent;
//                 Log.d("COCOS","COCOS--> monitorBatteryStatedianliang="+curBatter);
            }
        };
        batteryLevelFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        registerReceiver(batteryLevelRcvr, batteryLevelFilter);
    }
    // 获取安卓电量
    public static int getBattery(){
        Log.d("COCOS","COCOS getBattery"+curBatter);
        return curBatter;
    }
    // 判断gps 是否开启
    public static boolean checkGps() {
        LocationManager locationManager = (LocationManager) instance.getSystemService(Context.LOCATION_SERVICE);
        boolean gps = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        boolean network = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        if (gps || network) {
            Log.d("COCOS"," COCOS--> checkGps GpsCallback ====== 1");
            return true;
        }else{
            Log.d("COCOS"," COCOS--> checkGps GpsCallback ====== 0");
            return false;
        }
    }
    // 录音权限
    public static void checkVoice(){
        int minBuffer = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
        AudioRecord audioRecord = new AudioRecord(MediaRecorder.AudioSource.DEFAULT, 44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, (minBuffer * 100));
        try {
            audioRecord.startRecording();//检测是否可以进入初始化状态
        } catch (Exception e) {
            if (audioRecord != null) {
                audioRecord.release();
                audioRecord = null;
            }
            Log.d("COCOS","COCOS--> checkVoice 没有权限_1");
        }
        if (audioRecord.getRecordingState() != AudioRecord.RECORDSTATE_RECORDING) {
            Log.d("COCOS","COCOS--> checkVoice 没有权限_2");
        } else {
            if (audioRecord != null) {
                audioRecord.stop();
                audioRecord.release();
                audioRecord = null;
            }
            Log.d("COCOS","COCOS--> checkVoice 已获得录音权限");
        }
    }
    // 复制文本
    @SuppressLint("NewApi") public static void CopyText(String text ){
        Looper.prepare();
        ClipboardManager cm = (ClipboardManager)instance.getSystemService(CLIPBOARD_SERVICE);
        ClipData mClipData = ClipData.newPlainText("Label", (String)text);
        cm.setPrimaryClip(mClipData);
        Log.d("COCOS", "COCOS：AppActivity: CopyTextEnd: " + text);
    }
    // 保存图片到相册
    public static int imageToSavePhotos(String path){
        Log.d("COCOS", "--> imageToSavePhotos path: "+path);
        Bitmap bmp = null;
        try{
            bmp = BitmapFactory.decodeFile(path);
        }catch(Exception e){
            Log.d("COCOS","--> imageToSavePhotos open error");
        }
        // Log.d("cocos--> S2 Handler URL: ", bmp);
        String storePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "德胜";
        File appDir = new File(storePath);
        if (!appDir.exists()) {
            appDir.mkdir();
        }
        String fileName = System.currentTimeMillis() + ".jpg";
        // Log.d("cocos--> imageToSavePhotos fileName ", fileName);
        File file = new File(appDir, fileName);
        try {
            FileOutputStream fos = new FileOutputStream(file);
            //通过io流的方式来压缩保存图片
            boolean isSuccess = bmp.compress(Bitmap.CompressFormat.JPEG, 60, fos);
            fos.flush();
            fos.close();
            //保存图片后发送广播通知更新数据库
            Uri uri = Uri.fromFile(file);
            instance.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, uri));
            if (isSuccess) {
                return 0;
            } else {
                return 1;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return 1;
    }
    // 相册选取头像
    public static void openImageChoose() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/*");
        intent.putExtra("return-data", false);
        instance.startActivityForResult(intent, REQUEST_CODE_GETIMAGE_BYSDCARD);

    }
    private void cropImageUri(Uri uri, int outputX, int outputY, int requestCode) {
        CROP_FILE_PATH = instance.getFilesDir() + "/crop.png";
        try {
            FileOutputStream fos = openFileOutput("crop.png",MODE_WORLD_READABLE + MODE_WORLD_WRITEABLE);
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.d("COCOS","--> cropImageUri path="+CROP_FILE_PATH);
        Intent intent = new Intent("com.android.camera.action.CROP");
        intent.setDataAndType(uri, "image/*");
        intent.putExtra("crop", "true");
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        intent.putExtra("outputX", outputX);
        intent.putExtra("outputY", outputY);
        intent.putExtra("scale", true);
        Uri imageUri = Uri.parse("file://" + CROP_FILE_PATH);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
        intent.putExtra("return-data", false);
        intent.putExtra("outputFormat", Bitmap.CompressFormat.PNG.toString());
        intent.putExtra("noFaceDetection", true);
        instance.startActivityForResult(intent, requestCode);
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d("COCOS","COCOS--> AppActivity:onActivityResult xx");
        if (requestCode == REQUEST_CODE_GETIMAGE_BYSDCARD && resultCode == RESULT_OK && data != null) {
            Uri uri = data.getData();
            try{
                String path = FileUtils.getPath(AppActivity.getContext(), uri);
                Log.d("COCOS", "COCOS--> AppActivity:onActivityResult path=" + path);
                Uri newUri = Uri.parse("file://" + path); // 将绝对路径转换为URL
                cropImageUri(newUri, 200, 200, REQUEST_CODE_CROP_PICTURE);
            }catch (URISyntaxException e){
                Log.d("COCOS","COCOS OPEN PHOTO");
            }
        }else if (requestCode == REQUEST_CODE_CROP_PICTURE && resultCode == RESULT_OK) {
            Log.d("COCOS", "COCOS--> AppActivity:onActivityResult new path="+CROP_FILE_PATH);
//            JniCallCocos.GalleryCameraCallback(1, CROP_FILE_PATH);

        }
    }

}
