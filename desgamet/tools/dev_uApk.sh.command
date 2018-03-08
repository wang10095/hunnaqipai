#!/bin/bash
function apk()
{
	#
	# desgame
	#
	cd /data/work/desgamet/frameworks/runtime-src/Classes && echo '' >> AppDelegate.cpp  
	python /Users/ocean/Documents/cocos2d-x/tools/cocos2d-console/bin/cocos.py compile -p android -m debug --android-studio
	# echo '开始 apk 打包 --------->>>>>>>>>>> '
	# cd ~/Desktop/lyMaJiang  \
	# && cocos compile -p android -m release   \
	# &&  mv ~/Desktop/lyMaJiang/publish/android/syMaJiang-release-signed.apk $HOME/Sites/srclyMaJiang/apk_lymj.apk   \
	# && echo  "打包完成"
	# cd ~/Desktop/lyMaJiang && git reset --hard
}
# 更新包 begin # sleep 2s
apk

