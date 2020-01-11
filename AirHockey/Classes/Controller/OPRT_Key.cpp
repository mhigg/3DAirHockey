#include "OPRT_Key.h"

OPRT_Key::OPRT_Key()
{
}

OPRT_Key::OPRT_Key(cocos2d::Node * sprite)
{
	auto listener = cocos2d::EventListenerKeyboard::create();

	// ‰Ÿ‚µ‚½‚Æ‚«
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// ESC·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
		{
			// ³¨İÄŞ³‚ğ•Â‚¶‚é
			auto director = cocos2d::Director::getInstance();
			director->end();
		}
		// ¶·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::ON_NON;
		}
		// ‰E·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::ON_NON;
		}
		// ã·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::ON_NON;
		}
		// ‰º·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::ON_NON;
		}
		// ½Íß°½·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::ON_NON;
		}
		// Z·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::ON_NON;
		}
		return true;
	};

	// —£‚µ‚½‚Æ‚«
	listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// ¶·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::OFF_NON;
		}
		// ‰E·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::OFF_NON;
		}
		// ã·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::OFF_NON;
		}
		// ‰º·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::OFF_NON;
		}
		// ½Íß°½·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::OFF_NON;
		}
		// Z·°
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::OFF_NON;
		}
		return true;
	};
	sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);

	// 1ÌÚ°Ñ‚²‚Æ‚Éupdate‚ğ
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

		// ‰Ÿ‚µ‚½uŠÔ‚Ì‚Ü‚Ü‚È‚ç‰Ÿ‚µ‘±‚¯‚Ä‚¢‚éó‘Ô‚É
		if (state[data] == TRIGGER_STATE::ON_NON)
		{
			state[data] = TRIGGER_STATE::ON;
			break;
		}
		// —£‚µ‚½uŠÔ‚Ì‚Ü‚Ü‚È—£‚µ‘±‚¯‚Ä‚¢‚éó‘Ô‚É
		if (state[data] == TRIGGER_STATE::OFF_NON)
		{
			state[data] = TRIGGER_STATE::OFF;
			break;
		}
	}

	// ‰½‚©ˆê‚Â‚Å‚à‰Ÿ‚µ‚Ä‚¢‚½—p‚Ì½Ã°Ä
	state[INPUT_ID::ALL_KEY] = TRIGGER_STATE::OFF;

	for (auto data : INPUT_ID())
	{
		if (data == INPUT_ID::ALL_KEY)
		{
			continue;
		}
		// ‚Ç‚ê‚©‰Ÿ‚µ‚Ä‚¢‚½‚ç
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


