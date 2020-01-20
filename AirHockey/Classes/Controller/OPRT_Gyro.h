#pragma once
#include "jni.h"
#include "OPRT_State.h"

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



class OPRT_Gyro :
	public OPRT_State
{
public:
	OPRT_Gyro();
	OPRT_Gyro(cocos2d::Node* sprite);
	~OPRT_Gyro();
	void Update(void);
	cocos2d::Vec2 GetPoint(void) const;
private:
	cocos2d::Vec3 GetSensor(void);
	cocos2d::Vec3 _sensor;
	cocos2d::Vec3 _ratio;
	cocos2d::Vec2 _point;
};

