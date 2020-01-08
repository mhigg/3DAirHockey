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
	// 1度しか通らないようにする
	if (!initOnceFlag)
	{
		// ﾎﾞﾀﾝの登録
		Init();
	}

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

void OPRT_Touch::Init()
{
	// 画面解像度の取得
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	// 現在のｼｰﾝを取得
	auto scene = cocos2d::Director::getInstance()->getRunningScene();

	// 左矢印ｱｲｺﾝの設定
	cocos2d::ui::Button * leftIcon = cocos2d::ui::Button::create();
	// ﾀｯﾁ操作で反応するかどうか(trueで反応)
	leftIcon->setTouchEnabled(true);
	// ﾃｸｽﾁｬｰ読み込み(第1引数は通常時に表示,第2引数は押されたときに表示される)
	leftIcon->loadTextures("image/icon/LeftArrow.png", "image/icon/LeftArrow2.png");
	// 左下ぐらいに表示
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

	// ｼｰﾝに追加
	scene->getChildByName("Ui")->addChild(leftIcon);

	// 右矢印ｱｲｺﾝの設定
	cocos2d::ui::Button * rightIcon = cocos2d::ui::Button::create();
	rightIcon->setTouchEnabled(true);
	rightIcon->loadTextures("image/icon/RightArrow.png", "image/icon/RightArrow2.png");
	// 左矢印の隣に表示
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

	// ｼﾞｬﾝﾌﾟｱｲｺﾝの設定
	cocos2d::ui::Button * jumpIcon = cocos2d::ui::Button::create();
	jumpIcon->setTouchEnabled(true);
	jumpIcon->loadTextures("image/icon/JumpIcon.png", "image/icon/JumpIcon2.png");
	// 右下あたりに表示
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

	// ｼｮｯﾄｱｲｺﾝの設定
	cocos2d::ui::Button * shotIcon = cocos2d::ui::Button::create();
	shotIcon->setTouchEnabled(true);
	shotIcon->loadTextures("image/icon/ShotIcon.png", "image/icon/ShotIcon2.png");
	// 右下あたりに表示
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

	// 2回目通らないようにflag変更
	initOnceFlag = true;
}

