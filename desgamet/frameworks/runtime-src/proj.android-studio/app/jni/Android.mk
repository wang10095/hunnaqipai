LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared
LOCAL_LDLIBS += -llog
$(call import-add-path,$(LOCAL_PATH))
LOCAL_WHOLE_STATIC_LIBRARIES += GameLink_static
LOCAL_MODULE_FILENAME := libcocos2dlua

LOCAL_SRC_FILES := \
../../../Classes/AppDelegate.cpp \
../../../Classes/reader/AnimateClip.cpp \
../../../Classes/reader/AnimationClip.cpp \
../../../Classes/reader/AnimationManager.cpp \
../../../Classes/reader/CreatorReader.cpp \
../../../Classes/reader/CreatorReaderBinding.cpp \
../../../Classes/UpdateScene.cpp \
../../../Classes/PPCEX/common.cpp \
../../../Classes/PPCEX/helper/Md5.cpp \
../../../Classes/PPCEX/helper/ToolsHelper.cpp \
../../../Classes/PPCEX/pbc/alloc.c \
../../../Classes/PPCEX/pbc/array.c \
../../../Classes/PPCEX/pbc/bootstrap.c \
../../../Classes/PPCEX/pbc/context.c \
../../../Classes/PPCEX/pbc/decode.c \
../../../Classes/PPCEX/pbc/map.c \
../../../Classes/PPCEX/pbc/pattern.c \
../../../Classes/PPCEX/pbc/pbc-lua.c \
../../../Classes/PPCEX/pbc/proto.c \
../../../Classes/PPCEX/pbc/register.c \
../../../Classes/PPCEX/pbc/stringpool.c \
../../../Classes/PPCEX/pbc/varint.c \
../../../Classes/PPCEX/pbc/rmessage.c \
../../../Classes/PPCEX/pbc/wmessage.c \
../../../Classes/PPCEX/tea/tea_test.c \
../../../Classes/PPCEX/tea/tea_lua.c \
../../../Classes/PPCEX/lsqlite3/lsqlite3.c \
../../../Classes/PPCEX/lsqlite3/sqlite3.c \
hellolua/org_cocos2dx_lua_JniCallCocos.cpp \
hellolua/main.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../Classes \
$(LOCAL_PATH)/../../../cocos2d-x/external \
$(LOCAL_PATH)/GameLink/include \

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += cocos_curl_static
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2d-x)
$(call import-module, cocos/scripting/lua-bindings/proj.android)
$(call import-module,curl/prebuilt/android)
$(call import-module,GameLink)
# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
