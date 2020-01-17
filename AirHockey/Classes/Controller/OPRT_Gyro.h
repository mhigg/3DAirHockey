#pragma once
#include "jni.h"
#include "cocos2d.h"

struct SensorData
{
    float X;
    float Y;
    float Z;
};

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

    cocos2d::Vec2 Gyro();
private:
	cocos2d::Vec2 _point;
	cocos2d::Vec2 _force;
	SensorData _sensor;
};

