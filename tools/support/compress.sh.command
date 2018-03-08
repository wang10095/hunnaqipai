#!/bin/sh
curPath=$(cd `dirname $0`; pwd)
echo 'compress.sh.command-> '$compress
if [ $1 ]; then # 有参数
    cd $curPath'/PNGCompress' && sh PNGCompress $1
else
	if [ $compress ]; then
		cd $curPath'/PNGCompress' && sh PNGCompress $compress
	else
		cd $curPath'/PNGCompress' && sh PNGCompress '/data/work/tools/support/outPath'
	fi
fi
