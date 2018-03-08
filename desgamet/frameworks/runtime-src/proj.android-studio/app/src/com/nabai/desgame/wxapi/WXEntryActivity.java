package com.nabai.desgame.wxapi;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.ShowMessageFromWX;
import com.tencent.mm.opensdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import com.nabai.desgame.R;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import android.util.Log;
import org.cocos2dx.lua.AppActivity;
import java.net.URLEncoder;
public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	public static String GetCodeRequest = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code";
	private IWXAPI wxApi;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		Log.d("COCOS","WXEntryActivity onCreate");
		if(AppActivity.wxApi == null){
			AppActivity.wxApi = WXAPIFactory.createWXAPI(this, AppActivity.APP_ID, true);
			AppActivity.wxApi.registerApp(AppActivity.APP_ID);
		}
		AppActivity.wxApi.handleIntent(getIntent(), this);
		wxApi = WXAPIFactory.createWXAPI(this, AppActivity.APP_ID, false);
		wxApi.registerApp(AppActivity.APP_ID);
		wxApi.handleIntent(getIntent(), this);
    }
	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
		wxApi.handleIntent(intent, this);
		AppActivity.wxApi.handleIntent(intent, this);
	}
	@Override
	public void onReq(BaseReq req) {
		Log.d("COCOS","WXEntryActivity onReq->type="+req.getType());
		switch (req.getType()) {
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			goToShowMsg((ShowMessageFromWX.Req) req);
			break;
		default:
			break;
		}
		this.finish();
	}
	@Override
	public void onResp(BaseResp resp) {
		String result = "";
		switch (resp.errCode) {
		case BaseResp.ErrCode.ERR_OK:
			if (resp.getType() == ConstantsAPI.COMMAND_SENDAUTH) {
				String weixinCode = ((SendAuth.Resp) resp).code;
				result = "WeChat OK:" + weixinCode;
				AppActivity.code = weixinCode;
			}
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
			result = "WeChat Cancel";
			break;
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
			result = "WeChat Auth Denied";
			break;
		case BaseResp.ErrCode.ERR_UNSUPPORT:
			result = "WeChat unsupported";
			break;
		default:
			result = "WeChat Unknown";
			break;
		}
		Log.d("COCOS","WXEntryActivity onResp->result="+result+"errCode="+resp.errCode);
		this.finish();
	}
	private void goToShowMsg(ShowMessageFromWX.Req showReq) {
		Log.d("cocos WXEntryActivity","goToShowMsg");
		WXMediaMessage wxMsg = showReq.message;
		WXAppExtendObject obj = (WXAppExtendObject) wxMsg.mediaObject;

		StringBuffer msg = new StringBuffer();
		msg.append(obj.extInfo);

		Intent intent = new Intent(this,AppActivity.class);
		intent.putExtra("URL", msg.toString());

		startActivity(intent);
		finish();
	}
	public static String urlEnodeUTF8(String str) {
		String result = str;
		try {
			result = URLEncoder.encode(str, "UTF-8");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	public static String getCodeRequest(String code) {
		String result = null;
		result = GetCodeRequest.replace("APPID",
				urlEnodeUTF8(AppActivity.APP_ID));
		result = result.replace("SECRET",
				urlEnodeUTF8(AppActivity.WX_APP_SECRET));
		result = result.replace("CODE", urlEnodeUTF8(code));
		return result;
	}
}