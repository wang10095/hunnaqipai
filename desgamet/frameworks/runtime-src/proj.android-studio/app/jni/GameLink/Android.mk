LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ifly-prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libmsc.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := gl-prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libGameLink.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := GameLink_static
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../../cocos2d-x/cocos
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../../cocos2d-x/cocos/platform/android/jni
LOCAL_SRC_FILES := src/GameLink.cpp  src/GLMessage.cpp  src/GLTarget.cpp

LOCAL_SHARED_LIBRARIES := ifly-prebuilt gl-prebuilt

include $(BUILD_STATIC_LIBRARY)
