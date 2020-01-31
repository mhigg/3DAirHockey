#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME AirHockey)

project(${APP_NAME})

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()

include_directories(
        D:/Android/cricket/cricket-1.6.7/inc
)

link_directories(
        D:/Android/cricket/cricket-1.6.7/lib/android/ndk/${ANDROID_ABI}/debug
)

link_libraries(
        D:/Android/cricket/cricket-1.6.7/lib/android/ndk/${ANDROID_ABI}/debug/libck.a
)

include_directories(
        vendor/AndroidNdkSdk/
)

link_directories(
        vendor/AndroidNdkSdk/Common-cpp/lib/common-cpp-prebuilt
)

link_libraries(
        libcommon-cpp-static_debug_android_${ANDROID_ABI}_libc++.a
)

link_directories(
        vendor/AndroidNdkSdk/LoadBalancing-cpp/lib/loadbalancing-cpp-prebuilt
)

link_libraries(
        libloadbalancing-cpp-static_debug_android_${ANDROID_ABI}_libc++.a
)

link_directories(
        vendor/AndroidNdkSdk/Photon-cpp/lib/photon-cpp-prebuilt
)

link_libraries(
        libphoton-cpp-static_debug_android_${ANDROID_ABI}_libc++.a
)

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/Scene/TitleScene.cpp
     Classes/Scene/HostScene.cpp
     Classes/Scene/GuestScene.cpp
     Classes/Scene/GameScene.cpp
     Classes/Scene/ResultScene.cpp
     Classes/Controller/OPRT_State.cpp
     Classes/Controller/OPRT_Key.cpp
     Classes/Controller/OPRT_Touch.cpp
     Classes/Controller/MouseCtl.cpp
     Classes/Controller/OPRT_Network.cpp
     Classes/Obj/SpriteAffectDepth.cpp
     Classes/Obj/Player.cpp
     Classes/Obj/Ball.cpp
     Classes/Obj/BallAfter.cpp
     Classes/Obj/Shadow.cpp
     Classes/Obj/TrajectControl.cpp
     Classes/Obj/StageWall.cpp
     Classes/Effekseer/EffectMng.cpp
     Classes/Effekseer/Effekseer.cpp
     Classes/Effekseer/EffekseerNative.cpp
     Classes/Effekseer/EffekseerRendererNative.cpp
     Classes/Manager/AnimMng.cpp
     Classes/Manager/GameManager.cpp
     Classes/Manager/CCAudioMng.cpp
     Classes/Manager/AppInfo.cpp
     Classes/Manager/PointWithDepth.cpp
     Classes/Stage.cpp
     Classes/NetworkLogic.cpp
     Classes/ConsoleOut.cpp
     Classes/Collision.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/Scene/TitleScene.h
     Classes/Scene/HostScene.h
     Classes/Scene/GuestScene.h
     Classes/Scene/GameScene.h
     Classes/Scene/ResultScene.h
     Classes/Controller/OPRT_State.h
     Classes/Controller/OPRT_Key.h
     Classes/Controller/OPRT_Touch.h
     Classes/Controller/MouseCtl.h
     Classes/Controller/OPRT_Network.h
     Classes/Obj/SpriteAffectDepth.h
     Classes/Obj/Player.h
     Classes/Obj/Ball.h
     Classes/Obj/BallAfter.h
     Classes/Obj/Shadow.h
     Classes/Obj/TrajectControl.h
     Classes/Obj/StageWall.h
     Classes/Effekseer/EffectMng.h
     Classes/Effekseer/Effekseer.h
     Classes/Effekseer/EffekseerNative.h
     Classes/Effekseer/EffekseerRendererNative.h
     Classes/Manager/AnimMng.h
     Classes/Manager/GameManager.h
     Classes/Manager/CCAudioMng.h
     Classes/Manager/AppInfo.h
     Classes/Manager/PointWithDepth.h
     Classes/Stage.h
     Classes/NetworkLogic.h
     Classes/ConsoleOut.h
     Classes/OutputListener.h
     Classes/Collision.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
