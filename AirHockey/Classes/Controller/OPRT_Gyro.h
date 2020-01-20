#pragma once
#include "jni.h"
#include "cocos2d.h"


#ifndef __ANDROID_JNI__
#define __ANDROID_JNI__
#ifdef __cplusplus
extern "C" {
#endif
        JNIEXPORT float JNICALL getX();
		JNIEXPORT float JNICALL getY();
		JNIEXPORT float JNICALL getZ();
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ! __ANDROID_JNI__



class OPRT_Gyro
{
public:
	OPRT_Gyro();
	~OPRT_Gyro();

    cocos2d::Vec3 GetSensor();
private:
	cocos2d::Vec3 _Sensor;
};

