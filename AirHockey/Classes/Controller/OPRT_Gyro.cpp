#include "OPRT_Gyro.h"
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"

//NS_CC_BEGIN
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
	// X軸センサー
	JNIEXPORT float JNICALL getX()
	{
		cocos2d::JniMethodInfo methodInfo;
		if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getX", "()F"))
		{
		    float sensorX = 0;
		    sensorX = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);

			methodInfo.env->DeleteLocalRef(methodInfo.classID);
			return sensorX;
		}
		return 1000;
	}
	// Y軸センサー
	JNIEXPORT float JNICALL getY()
	{
		cocos2d::JniMethodInfo methodInfo;
		if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getY", "()F"))
		{
			float sensorY;
			sensorY = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
			return sensorY;
		}
		return 1000;
	}
	// Z軸センサー
	JNIEXPORT float JNICALL getZ()
	{
		cocos2d::JniMethodInfo methodInfo;
		if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getZ", "()F"))
		{
			float sensorZ;
			sensorZ = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
			return sensorZ;
		}
		return 1000;
	}
#ifdef __cplusplus
}
#endif
#else
#endif




OPRT_Gyro::OPRT_Gyro()
{
    _force = cocos2d::Vec2();
}


OPRT_Gyro::~OPRT_Gyro()
{
}

cocos2d::Vec2 OPRT_Gyro::Gyro()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_force.x = getX();
	_force.y = getY();
#else
#endif
	return _force;
}
