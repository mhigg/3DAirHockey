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
    _sensor = cocos2d::Vec3::ZERO;
}

OPRT_Gyro::OPRT_Gyro(cocos2d::Node * sprite)
{
	_sensor = cocos2d::Vec3::ZERO;
	_ratio = cocos2d::Vec3::ZERO;
}


OPRT_Gyro::~OPRT_Gyro()
{
}

void OPRT_Gyro::Update(void)
{
	// Sensorの取得
	auto sensor = GetSensor() / 50;
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	if (abs(sensor.x) > 0.0f)
	{
		_ratio.x += sensor.x;
	}
	if (abs(sensor.z) > 0.0f)
	{
		_ratio.y += sensor.y;
	}
	if (_ratio.x > 1)
	{
		_ratio.x = 1;
	}
	else if (_ratio.x < 0)
	{
		_ratio.x = 0;
	}
	if (_ratio.y > 1)
	{
		_ratio.y = 1;
	}
	if (_ratio.y < 0)
	{
		_ratio.y = 0;
	}
	_point = cocos2d::Vec2(visibleSize.width * _ratio.x, visibleSize.height * _ratio.y);
}

cocos2d::Vec2 OPRT_Gyro::GetPoint(void) const
{
	return _point;
}

cocos2d::Vec3 OPRT_Gyro::GetSensor(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_sensor.x = getX();
	_sensor.y = getY();
	_sensor.z = getZ();
#else
#endif

	return _sensor;
}
