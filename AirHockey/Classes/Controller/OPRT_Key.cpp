#include "OPRT_Key.h"

OPRT_Key::OPRT_Key()
{
}

OPRT_Key::OPRT_Key(cocos2d::Node * sprite)
{
	auto listener = cocos2d::EventListenerKeyboard::create();

	// �������Ƃ�
	listener->onKeyPressed = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// ESC��
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
		{
			// ����޳�����
			auto director = cocos2d::Director::getInstance();
			director->end();
		}
		// ����
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::ON_NON;
		}
		// �E��
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::ON_NON;
		}
		// �㷰
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::ON_NON;
		}
		// ����
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::ON_NON;
		}
		// ��߰���
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::ON_NON;
		}
		// Z��
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::ON_NON;
		}
		return true;
	};

	// �������Ƃ�
	listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool {
		// ����
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::OFF_NON;
		}
		// �E��
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::OFF_NON;
		}
		// �㷰
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			state[INPUT_ID::UP] = TRIGGER_STATE::OFF_NON;
		}
		// ����
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			state[INPUT_ID::DOWN] = TRIGGER_STATE::OFF_NON;
		}
		// ��߰���
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::OFF_NON;
		}
		// Z��
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Z)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::OFF_NON;
		}
		return true;
	};
	sprite->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);

	// 1�ڰт��Ƃ�update��
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

		// �������u�Ԃ̂܂܂Ȃ牟�������Ă����Ԃ�
		if (state[data] == TRIGGER_STATE::ON_NON)
		{
			state[data] = TRIGGER_STATE::ON;
			break;
		}
		// �������u�Ԃ̂܂܂ȗ��������Ă����Ԃ�
		if (state[data] == TRIGGER_STATE::OFF_NON)
		{
			state[data] = TRIGGER_STATE::OFF;
			break;
		}
	}

	// ������ł������Ă������p�̽ð�
	state[INPUT_ID::ALL_KEY] = TRIGGER_STATE::OFF;

	for (auto data : INPUT_ID())
	{
		if (data == INPUT_ID::ALL_KEY)
		{
			continue;
		}
		// �ǂꂩ�����Ă�����
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


