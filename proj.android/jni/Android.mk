LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Network/NetServer.cpp \
                   ../../Classes/Data/DataManager.cpp \
                   ../../Classes/Scenes/MainLayer.cpp \
                   ../../Classes/Scenes/LoadingLayer.cpp \
                   ../../Classes/Scenes/Hall.cpp \
                   ../../Classes/Scenes/SingleRoom.cpp \
                   ../../Classes/Scenes/SingleSubRoom.cpp \
                   ../../Classes/Scenes/SceneManager.cpp \
                   ../../Classes/CommonUI/CGCCBReader.cpp \
                   ../../Classes/CommonUI/CGControlButton.cpp \
                   ../../Classes/Logic/OnlineGameController.cpp \
                   ../../Classes/Logic/SingleGameController.cpp \
                   ../../Classes/Events/EventManager.cpp \
                   ../../Classes/Events/EventObserver.cpp \
                   ../../Classes/Events/Events.cpp \
                   ../../Classes/Events/GameEvents.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += pomelo_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libpomelo)
