#pragma once
#include "jni.h"
#include "OPRT_State.h"
#include "NetworkLogic.h"


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
	Oprt_Touch(cocos2d::Node* node, bool net, bool isHost);
	~Oprt_Touch();

	cocos2d::Vec2 GetPoint(void) const;
	void ResetSensor(void);
	void Update(void);		// 状態更新
	// Android用、Gyroの有効設定関数
	void GyroActive(bool setFlag);

private:
	// センサーを取得する
	cocos2d::Vec3 GetSensor(void);
	// タッチ座標
	cocos2d::Vec2 _touchPoint;
	// センサー値
	cocos2d::Vec3 _sensor;
	// センサーの割合
	cocos2d::Vec3 _ratio;

	// Gyroの状態フラグ(true:有効, false:無効)
	bool _active;
	// ネットワーク
	void Run(void);	// 通信の接続
	void addParticle(int playerNr, float x, float y);

	bool _isHost;	// ﾎｽﾄならtrue, ｹﾞｽﾄならfalse
	bool _swallowsTouches;
	NetworkLogic* _networkLogic;

};