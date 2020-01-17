LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := loadbalancing-cpp-static
#VisualGDBAndroid: AutoUpdateSourcesInNextLine
LOCAL_SRC_FILES := ../src/AuthenticationValues.cpp ../src/Client.cpp ../src/FriendInfo.cpp ../src/Internal/AuthenticationValuesSecretSetter.cpp ../src/Internal/MutablePlayerFactory.cpp ../src/Internal/MutableRoomFactory.cpp ../src/Internal/PlayerFactory.cpp ../src/Internal/PlayerMovementInformant.cpp ../src/Internal/PlayerMutableRoomPointerSetter.cpp ../src/Internal/PlayerPropertiesCacher.cpp ../src/Internal/PlayerPropertiesUpdateInformant.cpp ../src/Internal/RoomFactory.cpp ../src/Internal/RoomPropertiesCacher.cpp ../src/Internal/Utils.cpp ../src/Internal/PuncherClient.cpp ../src/LobbyStatsRequest.cpp ../src/LobbyStatsResponse.cpp ../src/MutablePlayer.cpp ../src/MutableRoom.cpp ../src/Peer.cpp ../src/Player.cpp ../src/RaiseEventOptions.cpp ../src/Room.cpp ../src/RoomOptions.cpp ../src/WebFlags.cpp
LOCAL_C_INCLUDES := ..
LOCAL_CFLAGS := -Werror-implicit-function-declaration -Wall -Wno-comment
include $(BUILD_STATIC_LIBRARY)

all: lib/loadbalancing-cpp-prebuilt/libloadbalancing-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI)$(MY_APP_STDLIB_SUFFIX)$(MY_APP_NORTTI_SUFFIX).a
lib/loadbalancing-cpp-prebuilt/libloadbalancing-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI)$(MY_APP_STDLIB_SUFFIX)$(MY_APP_NORTTI_SUFFIX).a : $(LOCAL_BUILT_MODULE)
	cp $< $@

include $(CLEAR_VARS)
LOCAL_MODULE := photon-cpp-static-prebuilt
LOCAL_SRC_FILES := ../../Photon-cpp/lib/photon-cpp-prebuilt/libphoton-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI)$(MY_APP_STDLIB_SUFFIX)$(MY_APP_NORTTI_SUFFIX).a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)../../../Photon-cpp/jni
include $(PREBUILT_STATIC_LIBRARY)