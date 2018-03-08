#!/bin/bash
project_root='/data/work/desgamet'
# child_res=('fastrun' 'hzmj','csmj') # 大厅包存在的子集 资源 (数组)
# child_res=('fastrun') # 大厅包存在的子集 资源 (数组)
child_res=() # 大厅包存在的子集 资源 (数组)
function preresres()
{
	isEmpty '/res/child'
	isResChild=$?
	if test $isResChild == 0; then
		echo '------------res/child  目录不存在 & 并创建'	
		mkdir $project_root'/res/child' 
	elif test $isResChild == 3; then
		echo '------------res/child  目录存在 & 并移动'
		mv $project_root'/res/child' $project_root'/child'
		mkdir $project_root'/res/child' 
	elif test $isResChild == 2; then
		echo '------------res/child 目录为空'
	fi

	#
	# 资源的根目录
	#
	begin_root=$project_root'/child/'
	end_root=$project_root'/res/child'	
	echo '准备移动的子集有-> '${child_res[@]}
	echo '移动的起始-> '$begin_root
	echo '移动的结束-> '$end_root
	if test ${#child_res[@]} > 0;then
		echo '移动子集目录res'
		for var in ${child_res[@]}; do
			echo '移动这个子集-> '$begin_root$var
			cp -r $begin_root$var $end_root
		done
	fi
}
function preressrc()
{
	#
	# 代码的根目录
	#
	isEmpty '/src/child'
	isSrcChild=$?
	if test $isSrcChild == 0; then
		echo '------------src/child  目录不存在 & 并创建'	
		mkdir $project_root'/src/child' 
	elif test $isSrcChild == 3; then
		echo '------------src/child  目录存在 & 并移动'
		mv $project_root'/src/child' $project_root'/childsrc'
		mkdir $project_root'/src/child' 
	elif test $isSrcChild == 2; then
		echo '------------src/child 目录为空'
	fi
	beginsrc_root=$project_root'/childsrc/'
	endsrc_root=$project_root'/src/child'
	echo '移动的起始-> '$beginsrc_root
	echo '移动的结束-> '$endsrc_root
	if test ${#child_res[@]} > 0;then
		echo '移动子集目录src'
		for var in ${child_res[@]}; do
			echo '移动这个子集-> '$beginsrc_root$var
			cp -r $beginsrc_root$var $endsrc_root
		done
	fi
}
function recres()
{
	echo '准备大厅子集 资源 复合'
	cd $project_root'/res' && rm -rf child
	mv $project_root'/child' $project_root'/res/child'
}
function recsrc()
{
	echo '准备大厅子集 代码 复合'
	cd $project_root'/src' && rm -rf child
	mv $project_root'/childsrc' $project_root'/src/child'
}
#  exit 1 
#  0: 目录不存在
#  1: 是一个文件
#  2：目录为空
#  3：目录不为空
function isEmpty()
{
	# echo $1
	path=$project_root$1
	if test -d $path; then
		if test !  "`ls $path`"; then
			return 2
		else
			return 3
		fi
	else
		if test -f $path; then
			return 1
		else
			return 0
		fi
	fi
}
function deploy()
{
	#
	# desgame
	#

	# preresres
	# preressrc
	recres
	recsrc
}
deploy

