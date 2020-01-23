#include "GuestScene.h"
#include "GameScene.h"
#include "ConsoleOut.h"

USING_NS_CC;

GuestScene::~GuestScene()
{
}

cocos2d::Scene * GuestScene::createScene()
{
	return GuestScene::create();
}

bool GuestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// 画面解像度の取得
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 座標の取得
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 右上のｼｬｯﾄﾀﾞｳﾝﾎﾞﾀﾝの作成
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GuestScene::menuCloseCallback, this));
	auto closeItemVec =
		Vec2{ origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
			  origin.y + visibleSize.height - closeItem->getContentSize().height / 2 };
	closeItem->setPosition(closeItemVec);
	auto menu = Menu::create(closeItem, NULL);
	menu->setName("Menu");
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	/// シーン遷移用のボタンの作成
	auto sceneItem = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(GuestScene::ChangeScene, this));
	auto sceneItemVec = Vec2(origin.x + visibleSize.width - sceneItem->getContentSize().width / 2,
		sceneItem->getContentSize().height / 2);
	sceneItem->setPosition(sceneItemVec);
	auto sceneMenu = Menu::create(sceneItem, 0);
	sceneMenu->setName("sceneMenu");
	sceneMenu->setPosition(Vec2::ZERO);
	this->addChild(sceneMenu, static_cast<int>(LayerNum::FRONT));

	/// 現在のシーンを表すテキスト
	auto label = Label::create("Guest", "Arial", 60);
	label->setPosition(Vec2(label->getContentSize().width / 2,
							visibleSize.height - label->getContentSize().height / 2));
	this->addChild(label);

	networkLogic = new NetworkLogic(&ConsoleOut::get());

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	return true;
}

void GuestScene::update(float dt)
{
	networkLogic->run();
	switch (networkLogic->getState()) {
	case STATE_CONNECTED:
	case STATE_LEFT:
		// ルームが存在すればジョイン、なければ作成する
		if (networkLogic->isRoomExists()) {
			networkLogic->setLastInput(INPUT_2);
		}
		else {
			networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// 接続が切れたら再度接続
		networkLogic->connect();
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

void GuestScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(), Color3B::WHITE));
}

void GuestScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}
