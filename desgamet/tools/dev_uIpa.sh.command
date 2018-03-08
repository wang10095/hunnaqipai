#!/bin/bash
function ipa()
{
	#
	# desgame
	#
	cd /data/work/desgamet/frameworks/runtime-src/Classes && echo '' >> AppDelegate.cpp  
	CODE_SIGN_IDENTITY='iPhone Developer: SUN JIAXIANG (CPLB3R63M5)'
	PROVISIONING_PROFILE='f609319b-8966-45e2-8175-7f1954df7aac' #des tuisong
	CONFIGURATION_BUILD_DIR='/data/work/desgamet/simulator/ios'
	projectPath='/data/work/desgamet/frameworks/runtime-src/proj.ios_mac/desgamet.xcodeproj'
	archivePath='/data/work/desgamet/simulator/ios/desgamet-mobile.xcarchive'
	targetPath='/data/work/desgamet/simulator/ios/desgamet-mobile.app'
	exportPath='/data/work/desgamet/simulator/ios/'
	exportPlist='/data/work/desgamet/tools/dev_ExportOptions.plist'
	schemeName='desgamet-mobile'
	# 新的打包方式
	# xcodebuild archive -project $projectPath -scheme $schemeName -configuration Debug -archivePath $archivePath CONFIGURATION_BUILD_DIR="$CONFIGURATION_BUILD_DIR" CODE_SIGN_IDENTITY="$CODE_SIGN_IDENTITY" PROVISIONING_PROFILE="$PROVISIONING_PROFILE"
	# xcodebuild -exportArchive -archivePath $archivePath -exportPath $exportPath -exportOptionsPlist $exportPlist -allowProvisioningUpdates
	# 老的打包方式
	xcodebuild -project $projectPath -target $schemeName -configuration Debug -sdk iphoneos build CONFIGURATION_BUILD_DIR="$CONFIGURATION_BUILD_DIR" CODE_SIGN_IDENTITY="$CODE_SIGN_IDENTITY" PROVISIONING_PROFILE="$PROVISIONING_PROFILE"
	xcrun -sdk iphoneos packageapplication -v $targetPath -o $exportPath"desgame.ipa"

	# 上传appstore 建议用xcode
}
ipa
