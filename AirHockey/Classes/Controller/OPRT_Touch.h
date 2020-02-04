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

struct Oprt_Touch :
	public OPRT_State
{
public:
	Oprt_Touch();
	Oprt_Touch(cocos2d::Node* node);
	~Oprt_Touch();

	cocos2d::Vec2 GetPoint(void) const;
	void ResetSensor(void);
	void Update(void);		// 状態更新
	void GyroActive(bool setFlag);

private:
	cocos2d::Vec3 GetSensor(void);
	// タッチ座標
	cocos2d::Vec2 _touchPoint;
	// センサー値
	cocos2d::Vec3 _sensor;
	// センサーの割合
	cocos2d::Vec3 _ratio;
	cocos2d::Vec2 _point;

	// Gyroの状態フラグ
	bool _active;
};