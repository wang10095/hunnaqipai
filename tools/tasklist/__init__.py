#coding=utf-8
import os
import sys
actions={'keys':[],'values':{}}
def add_action(key,name,callback):
    actions['keys'].append(key)
    actions['values'][key] = {'name':name,'callback':callback}


# 添加自定义函数, 建议自定义函数以action 或 expect 或 什么什么起头
import action_update 
# add_action("datingdev","大厅更新_200",action_update.runDatingDev)
add_action("datingdebug","大厅更新_debug",action_update.runDatingDebug)
# add_action("datingonline","大厅更新_online",action_update.runDatingOnline)
add_action("datingsvn","大厅提交资源",action_update.submitsvn)


import child_update 
# add_action("csmjdev","长沙麻将更新_200",child_update.runcsmjDev)
add_action("csmjdebug","长沙麻将更新_debug",child_update.runcsmjDebug)
# add_action("csmjonline","长沙麻将更新_online",child_update.runcsmjOnline)

# add_action("hzmjdev","红中麻将更新_200",child_update.runhzmjDev)
add_action("hzmjdebug","红中麻将更新_debug",child_update.runhzmjDebug)
# add_action("hzmjonline","红中麻将更新_online",child_update.runhzmjOnline)

# add_action("fstrdev","跑得快更新_200",child_update.runfstrDev)
add_action("fstrdebug","跑得快更新_debug",child_update.runfstrDebug)
# add_action("fstronline","跑得快更新_online",child_update.runfstrOnline)

# add_action("hsphzdev","汉寿跑胡子更新_200",child_update.runhsphzDev)
add_action("hsphzdebug","汉寿跑胡子更新_debug",child_update.runhsphzDebug)
# add_action("hsphzonline","汉寿跑胡子更新_online",child_update.runhsphzOnline)

add_action("lsmjdebug","梨树麻将更新_debug",child_update.runlsmjDebug)

add_action("cdphzdebug","常德跑胡子更新_debug",child_update.runcdphzDebug)

import action_tool

# add_action("compressPng","压缩图片",action_tool.compressPng)
# add_action("scalePng","缩放图片",action_tool.scalePng)
# add_action("renamePng","重命名图片-序列帧",action_tool.renamePng)

add_action("testKey1","这是一个测试1",action_tool.run1)
add_action("testKey2","这是一个测试2",action_tool.run7)



