#!/bin/bash
function deploy()
{
	echo --------------- 大厅 res 目录
	cd /data/work/desgamet/res && echo && pwd && echo && svn st && svn ci -m "提交res版本"
	echo --------------- 子集 res csmj 目录
	cd /data/work/desgamet/res/child/csmj && echo && pwd && echo && svn st && svn ci -m "提交csmj版本"
	echo --------------- 子集 res hzmj 目录
	cd /data/work/desgamet/res/child/hzmj && echo && pwd && echo && svn st && svn ci -m "提交hzmj版本"
	echo --------------- 子集 res fastrun 目录
	cd /data/work/desgamet/res/child/fastrun && echo && pwd && echo && svn st && svn ci -m "提交fastrun版本"
	echo --------------- 子集 res hsphz 目录
	cd /data/work/desgamet/res/child/hsphz && echo && pwd && echo && svn st && svn ci -m "提交hsphz版本"
}
deploy

