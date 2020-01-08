#include "OPRT_Touch.h"
#include "ui/CocosGUI.h"


OPRT_Touch::OPRT_Touch()
{
}

OPRT_Touch::OPRT_Touch(cocos2d::Node * sprite)
{
}

OPRT_Touch::~OPRT_Touch()
{
}

void OPRT_Touch::Update(void)
{
	// 1�x�����ʂ�Ȃ��悤�ɂ���
	if (!initOnceFlag)
	{
		// ���݂̓o�^
		Init();
	}

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

void OPRT_Touch::Init()
{
	// ��ʉ𑜓x�̎擾
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	// ���݂̼�݂��擾
	auto scene = cocos2d::Director::getInstance()->getRunningScene();

	// ����󱲺݂̐ݒ�
	cocos2d::ui::Button * leftIcon = cocos2d::ui::Button::create();
	// �������Ŕ������邩�ǂ���(true�Ŕ���)
	leftIcon->setTouchEnabled(true);
	// ø�����ǂݍ���(��1�����͒ʏ펞�ɕ\��,��2�����͉����ꂽ�Ƃ��ɕ\�������)
	leftIcon->loadTextures("image/icon/LeftArrow.png", "image/icon/LeftArrow2.png");
	// �������炢�ɕ\��
	leftIcon->setAnchorPoint(cocos2d::Vec2{ 0,0 });
	leftIcon->setPosition({ 10,10 });
	leftIcon->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {

		if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::ON_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::MOVED)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::ON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::OFF_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::CANCELED)
		{
			state[INPUT_ID::LEFT] = TRIGGER_STATE::OFF_NON;
		}
	});

	// ��݂ɒǉ�
	scene->getChildByName("Ui")->addChild(leftIcon);

	// �E��󱲺݂̐ݒ�
	cocos2d::ui::Button * rightIcon = cocos2d::ui::Button::create();
	rightIcon->setTouchEnabled(true);
	rightIcon->loadTextures("image/icon/RightArrow.png", "image/icon/RightArrow2.png");
	// �����ׂ̗ɕ\��
	rightIcon->setAnchorPoint(cocos2d::Vec2{ 0,0 });
	rightIcon->setPosition({ rightIcon->getContentSize().width + 50,10 });
	rightIcon->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {

		if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::ON_NON;
		}

		if (type == cocos2d::ui::Widget::TouchEventType::MOVED)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::ON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::OFF_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::CANCELED)
		{
			state[INPUT_ID::RIGHT] = TRIGGER_STATE::OFF_NON;
		}
	});
	scene->getChildByName("Ui")->addChild(rightIcon);

	// �ެ��߱��݂̐ݒ�
	cocos2d::ui::Button * jumpIcon = cocos2d::ui::Button::create();
	jumpIcon->setTouchEnabled(true);
	jumpIcon->loadTextures("image/icon/JumpIcon.png", "image/icon/JumpIcon2.png");
	// �E��������ɕ\��
	jumpIcon->setAnchorPoint(cocos2d::Vec2{ 1,0 });
	jumpIcon->setPosition({ visibleSize.width - 10,10 });
	jumpIcon->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {

		if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::ON_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::MOVED)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::ON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::OFF_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::CANCELED)
		{
			state[INPUT_ID::BTN_1] = TRIGGER_STATE::OFF_NON;
		}
	});
	scene->getChildByName("Ui")->addChild(jumpIcon);

	// ���ı��݂̐ݒ�
	cocos2d::ui::Button * shotIcon = cocos2d::ui::Button::create();
	shotIcon->setTouchEnabled(true);
	shotIcon->loadTextures("image/icon/ShotIcon.png", "image/icon/ShotIcon2.png");
	// �E��������ɕ\��
	shotIcon->setAnchorPoint(cocos2d::Vec2{ 1,0 });
	shotIcon->setPosition({ visibleSize.width - jumpIcon->getContentSize().width - 30,10 });
	shotIcon->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {

		if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::ON_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::MOVED)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::ON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::OFF_NON;
		}
		if (type == cocos2d::ui::Widget::TouchEventType::CANCELED)
		{
			state[INPUT_ID::BTN_2] = TRIGGER_STATE::OFF_NON;
		}
	});
	scene->getChildByName("Ui")->addChild(shotIcon);

	// 2��ڒʂ�Ȃ��悤��flag�ύX
	initOnceFlag = true;
}

