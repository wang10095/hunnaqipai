#!/bin/bash
function deploy()
{
	echo --------------- 大厅目录
	cd /data/work/desgamet && echo && pwd && echo && git status && git reset --hard && git pull
	echo --------------- 大厅 src 目录
	cd /data/work/desgamet/src && echo && pwd && echo && git status && git pull
	echo --------------- 子集 csmj 目录
	cd /data/work/desgamet/src/child/csmj && echo && pwd && echo && git status && git pull
	echo --------------- 子集 hzmj 目录
	cd /data/work/desgamet/src/child/hzmj && echo && pwd && echo && git status && git pull
	echo --------------- 子集 fastrun 目录
	cd /data/work/desgamet/src/child/fastrun && echo && pwd && echo && git status && git pull
	echo --------------- 子集 hsphz 目录
	cd /data/work/desgamet/src/child/hsphz && echo && pwd && echo && git status && git pull

	echo --------------- 大厅 res 目录
	cd /data/work/desgamet/res && echo && pwd && echo && svn st && svn up
	echo --------------- 子集 res csmj 目录
	cd /data/work/desgamet/res/child/csmj && echo && pwd && echo && svn st && svn up
	echo --------------- 子集 res hzmj 目录
	cd /data/work/desgamet/res/child/hzmj && echo && pwd && echo && svn st && svn up
	echo --------------- 子集 res fastrun 目录
	cd /data/work/desgamet/res/child/fastrun && echo && pwd && echo && svn st && svn up
	echo --------------- 子集 res hsphz 目录
	cd /data/work/desgamet/res/child/hsphz && echo && pwd && echo && svn st && svn up

	echo --------------- 工具目录
	cd /data/work/tools && echo && pwd && echo && git status && git pull
}
deploy

