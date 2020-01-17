#include "OPRT_Key.h"

OPRT_Key::OPRT_Key()
{
}

OPRT_Key::OPRT_Key(cocos2d::Node * sprite)
{
	auto listener = cocos2d::EventListenerKeyboard::create();

	// 押したとき
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// ESCｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
		{
			// ｳｨﾝﾄﾞｳを閉じる
			auto director = cocos2d::Director::getInstance();
			director->end();
		}
		// 左ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::ON_NON;
		}
		// 右ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::ON_NON;
		}
		// 上ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::ON_NON;
		}
		// 下ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::ON_NON;
		}
		// ｽﾍﾟｰｽｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::ON_NON;
		}
		// Zｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::ON_NON;
		}
		return true;
	};

	// 離したとき
	listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// 左ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::OFF_NON;
		}
		// 右ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::OFF_NON;
		}
		// 上ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::OFF_NON;
		}
		// 下ｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::OFF_NON;
		}
		// ｽﾍﾟｰｽｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::OFF_NON;
		}
		// Zｷｰ
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::OFF_NON;
		}
		return true;
	};
	sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);

	// 1ﾌﾚｰﾑごとにupdateを
	this->cocos2d::Node::scheduleUpdate();
}


OPRT_Key::~OPRT_Key()
{
}

void OPRT_Key::Update()
{
	for (auto data : INPUT_ID())
	{
		if (data == INPUT_ID::ALL_KEY)
		{
			continue;
		}

		// 押した瞬間のままなら押し続けている状態に
		if (state[data] == TRIGGER_STATE::ON_NON)
		{
			state[data] = TRIGGER_STATE::ON;
			break;
		}
		// 離した瞬間のままな離し続けている状態に
		if (state[data] == TRIGGER_STATE::OFF_NON)
		{
			state[data] = TRIGGER_STATE::OFF;
			break;
		}
	}

	// 何か一つでも押していた時用のｽﾃｰﾄ
	state[INPUT_ID::ALL_KEY] = TRIGGER_STATE::OFF;

	for (auto data : INPUT_ID())
	{
		if (data == INPUT_ID::ALL_KEY)
		{
			continue;
		}
		// どれか押していたら
		if (state[data] <= TRIGGER_STATE::ON)
		{
			state[INPUT_ID::ALL_KEY] = TRIGGER_STATE::ON;
			break;
		}
	}
}

cocos2d::Vec2 OPRT_Key::GetPoint(void) const
{
	return _point;
}


