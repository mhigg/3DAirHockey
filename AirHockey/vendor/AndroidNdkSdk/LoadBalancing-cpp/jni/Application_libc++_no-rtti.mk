APP_MODULES := loadbalancing-cpp-static
APP_ABI := all
APP_CFLAGS := -D_EG_ANDROID_PLATFORM
APP_CPPFLAGS := -fno-rtti
MY_APP_NORTTI_SUFFIX := _no-rtti
APP_STL := c++_static
MY_APP_STDLIB_SUFFIX := _libc++