cmake_minimum_required(VERSION 3.6)

project(photon)

set(PHOTON_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "PHOTON_ROOT_PATH:" ${PHOTON_ROOT_PATH})
message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "ANDROID_ABI:" ${ANDROID_ABI})

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(PHOTON_BUILD_TYPE debug)
else()
    set(PHOTON_BUILD_TYPE release)
endif()

message(STATUS "PHOTON_BUILD_TYPE:" ${PHOTON_BUILD_TYPE})
message(STATUS "COMMON_PATH:" ${PHOTON_ROOT_PATH}/Common-cpp/lib/common-cpp-prebuilt/libcommon-cpp-static_${PHOTON_BUILD_TYPE}_android_${ANDROID_ABI}.a)

add_library(photon_common STATIC IMPORTED)
set_target_properties(photon_common PROPERTIES
    IMPORTED_LOCATION "${PHOTON_ROOT_PATH}/Common-cpp/lib/common-cpp-prebuilt/libcommon-cpp-static_${PHOTON_BUILD_TYPE}_android_${ANDROID_ABI}_libc++.a"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    LINKER_LANGUAGE "CXX")

add_library(photon STATIC IMPORTED)
set_target_properties(photon PROPERTIES
    IMPORTED_LOCATION "${PHOTON_ROOT_PATH}/Photon-cpp/lib/photon-cpp-prebuilt/libphoton-cpp-static_${PHOTON_BUILD_TYPE}_android_${ANDROID_ABI}_libc++.a"
    LINKER_LANGUAGE "CXX"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    )

set(PHOTON_LOAD_SRC ${PHOTON_ROOT_PATH}/LoadBalancing-cpp)

set(PHOTON_HEADER
    ${PHOTON_LOAD_SRC}/inc/AuthenticationValues.h
    ${PHOTON_LOAD_SRC}/inc/Client.h
    ${PHOTON_LOAD_SRC}/inc/FriendInfo.h
    ${PHOTON_LOAD_SRC}/inc/internal/AuthenticationValuesSecretSetter.h
    ${PHOTON_LOAD_SRC}/inc/internal/MutablePlayerFactory.h
    ${PHOTON_LOAD_SRC}/inc/internal/MutableRoomFactory.h
    ${PHOTON_LOAD_SRC}/inc/internal/PlayerFactory.h
    ${PHOTON_LOAD_SRC}/inc/internal/PlayerMovementInformant.h
    ${PHOTON_LOAD_SRC}/inc/internal/PlayerMutableRoomPointerSetter.h
    ${PHOTON_LOAD_SRC}/inc/internal/PlayerPropertiesCacher.h
    ${PHOTON_LOAD_SRC}/inc/internal/PlayerPropertiesUpdateInformant.h
    ${PHOTON_LOAD_SRC}/inc/internal/PuncherClient.h
    ${PHOTON_LOAD_SRC}/inc/internal/RoomFactory.h
    ${PHOTON_LOAD_SRC}/inc/internal/RoomPropertiesCacher.h
    ${PHOTON_LOAD_SRC}/inc/internal/Utils.h
    ${PHOTON_LOAD_SRC}/inc/LobbyStatsRequest.h
    ${PHOTON_LOAD_SRC}/inc/LobbyStatsResponse.h
    ${PHOTON_LOAD_SRC}/inc/MutablePlayer.h
    ${PHOTON_LOAD_SRC}/inc/MutableRoom.h
    ${PHOTON_LOAD_SRC}/inc/Peer.h
    ${PHOTON_LOAD_SRC}/inc/Player.h
    ${PHOTON_LOAD_SRC}/inc/RaiseEventOptions.h
    ${PHOTON_LOAD_SRC}/inc/Room.h
    ${PHOTON_LOAD_SRC}/inc/RoomOptions.h
    ${PHOTON_LOAD_SRC}/inc/WebFlags.h
    )

set(PHOTON_SRC
    ${PHOTON_LOAD_SRC}/src/AuthenticationValues.cpp
    ${PHOTON_LOAD_SRC}/src/Client.cpp
    ${PHOTON_LOAD_SRC}/src/FriendInfo.cpp
    ${PHOTON_LOAD_SRC}/src/internal/AuthenticationValuesSecretSetter.cpp
    ${PHOTON_LOAD_SRC}/src/internal/MutablePlayerFactory.cpp
    ${PHOTON_LOAD_SRC}/src/internal/MutableRoomFactory.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PlayerFactory.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PlayerMovementInformant.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PlayerMutableRoomPointerSetter.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PlayerPropertiesCacher.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PlayerPropertiesUpdateInformant.cpp
    ${PHOTON_LOAD_SRC}/src/internal/PuncherClient.cpp
    ${PHOTON_LOAD_SRC}/src/internal/RoomFactory.cpp
    ${PHOTON_LOAD_SRC}/src/internal/RoomPropertiesCacher.cpp
    ${PHOTON_LOAD_SRC}/src/internal/Utils.cpp
    ${PHOTON_LOAD_SRC}/src/LobbyStatsRequest.cpp
    ${PHOTON_LOAD_SRC}/src/LobbyStatsResponse.cpp
    ${PHOTON_LOAD_SRC}/src/MutablePlayer.cpp
    ${PHOTON_LOAD_SRC}/src/MutableRoom.cpp
    ${PHOTON_LOAD_SRC}/src/Peer.cpp
    ${PHOTON_LOAD_SRC}/src/Player.cpp
    ${PHOTON_LOAD_SRC}/src/RaiseEventOptions.cpp
    ${PHOTON_LOAD_SRC}/src/Room.cpp
    ${PHOTON_LOAD_SRC}/src/RoomOptions.cpp
    ${PHOTON_LOAD_SRC}/src/WebFlags.cpp
    )

list(APPEND PHOTON_SRC ${PHOTON_HEADER})
add_library(photon_loadbalancing SHARED ${PHOTON_SRC})
target_link_libraries(photon_loadbalancing photon photon_common)
target_include_directories(photon_loadbalancing
        PRIVATE ${PHOTON_ROOT_PATH}
)