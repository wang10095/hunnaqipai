#!/bin/bash
function apk()
{
	#
	# desgame
	#
	cd /data/work/desgamet/frameworks/runtime-src/Classes && echo '' >> AppDelegate.cpp  
	python /Users/nabai_dev/Downloads/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos.py compile -p android -m debug --android-studio
}
# 更新包 begin # sleep 2s
apk

