#include "OPRT_Touch.h"
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

Oprt_Touch::Oprt_Touch()
{
}

Oprt_Touch::Oprt_Touch(cocos2d::Node * node)
{
	_isHost = false;
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	_active = true;
	_touchPoint = cocos2d::Vec2((origin.x + visibleSize.width) / 2, origin.y + visibleSize.height / 2);
	_point = cocos2d::Vec2((origin.x + visibleSize.width) / 2, origin.y + visibleSize.height / 2);
	_sensor = cocos2d::Vec3::ZERO;
	_ratio = cocos2d::Vec3(0.5f, 0.5f, 0.5f);
	// 初期化
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	// 押した瞬間
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{	
		_touchPoint = touch->getLocation();
		return true;
	};
	// 動かしている間
	listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		_touchPoint = touch->getLocation();
	};
	// 離した瞬間
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		_touchPoint = touch->getLocation();
	};
	node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
	this->scheduleUpdate();
}

Oprt_Touch::~Oprt_Touch()
{
}

cocos2d::Vec2 Oprt_Touch::GetPoint(void) const
{
	return _point;
}

void Oprt_Touch::ResetSensor(void)
{
	_ratio = cocos2d::Vec3::ZERO;
}

void Oprt_Touch::Update(void)
{

	// Gyro操作か、タッチ操作
	if (_active == true)
	{
		// Sensorの取得
		auto sensor = GetSensor() / 25;
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
		else if (_ratio.y < 0)
		{
			_ratio.y = 0;
		}
		_point = cocos2d::Vec2(visibleSize.width * _ratio.x, visibleSize.height * _ratio.y);
	}
	else
	{
		_point = _touchPoint;

	}

	if (_networkLogic != nullptr)
	{
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, _point.x);
		eventContent->put<int, float>(2, _point.y);
		_networkLogic->sendEvent(1, eventContent);

		Run();

		// データの送信：EventDispacherの命令に任せる→あえてここに書かなくていい

		// ここでﾃﾞｰﾀ受信
		// GetPointで渡す_pointの値の更新
		while (!_networkLogic->eventQueue.empty())
		{
			std::array<float, 3>arr = _networkLogic->eventQueue.front();
			_networkLogic->eventQueue.pop();

			int playerNr = static_cast<int>(arr[0]);
			_point.x = arr[1];
			_point.y = arr[2];
			CCLOG("%d, %f, %f", playerNr, _point.x, _point.y);
		}
	}
}

void Oprt_Touch::GyroActive(bool setFlag)
{
	_active = setFlag;
}

cocos2d::Vec3 Oprt_Touch::GetSensor(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_sensor.x = getX();
	_sensor.y = getY();
	_sensor.z = getZ();
#else
#endif
	return _sensor;
}

void Oprt_Touch::Run(void)
{
	_networkLogic->run();
	switch (_networkLogic->getState())
	{
	case STATE_CONNECTED:
	case STATE_LEFT:
		if (_isHost)
		{
			// ゲスト側で、ルームが存在すればジョイン
			if (_networkLogic->isRoomExists())
			{
				_networkLogic->setLastInput(INPUT_2);
			}
		}
		else
		{
			// ホスト側ならルーム作成
			_networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// 接続が切れたら再度接続
		_networkLogic->connect();
		break;
	case STATE_CONNECTING:
	case STATE_JOINING:
	case STATE_JOINED:
	case STATE_LEAVING:
	case STATE_DISCONNECTING:
	default:
		break;
	}
}

void Oprt_Touch::addParticle(int playerNr, float x, float y)
{
}
