#include "HostScene.h"
#include "GameScene.h"
#include "ConsoleOut.h"

USING_NS_CC;

HostScene::~HostScene()
{
}

cocos2d::Scene * HostScene::createScene()
{
	return HostScene::create();
}

bool HostScene::init()
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
		CC_CALLBACK_1(HostScene::menuCloseCallback, this));
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
		CC_CALLBACK_1(HostScene::ChangeScene, this));
	auto sceneItemVec = Vec2(origin.x + visibleSize.width - sceneItem->getContentSize().width / 2,
		sceneItem->getContentSize().height / 2);
	sceneItem->setPosition(sceneItemVec);
	auto sceneMenu = Menu::create(sceneItem, 0);
	sceneMenu->setName("sceneMenu");
	sceneMenu->setPosition(Vec2::ZERO);
	this->addChild(sceneMenu, static_cast<int>(LayerNum::FRONT));

	/// 現在のシーンを表すテキスト
	auto label = Label::create("Host", "Arial", 60);
	label->setPosition(Vec2(label->getContentSize().width / 2,
							visibleSize.height - label->getContentSize().height / 2));
	this->addChild(label);

	networkLogic = new NetworkLogic(&ConsoleOut::get());
	ExitGames::LoadBalancing::AuthenticationValues().setUserID(L"Host");

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();
	return true;
}

void HostScene::update(float dt)
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

	while (!networkLogic->eventQueue.empty()) {
		std::array<float, 3> arr = networkLogic->eventQueue.front();
		networkLogic->eventQueue.pop();

		int playerNr = static_cast<int>(arr[0]);
		float x = arr[1];
		float y = arr[2];
		CCLOG("%d, %f, %f", playerNr, x, y);

		this->addParticle(playerNr, x, y);
	}
}

void HostScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(), Color3B::WHITE));
}

void HostScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}

bool HostScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (networkLogic->playerNr) {
		this->addParticle(networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);

		// イベント（タッチ座標）を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, touch->getLocation().x);
		eventContent->put<int, float>(2, touch->getLocation().y);
		networkLogic->sendEvent(1, *eventContent);
	}
	
	return true;
}

void HostScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void HostScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void HostScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void HostScene::addParticle(int playerNr, float x, float y)
{
	ParticleSystem* particle;
	switch (playerNr) {
	case 1:
		particle = ParticleFire::create();
		break;
	case 2:
		particle = ParticleSmoke::create();
		break;
	case 3:
		particle = ParticleFlower::create();
		break;
	default:
		particle = ParticleSun::create();
		break;
	}
	particle->setDuration(0.1);
	particle->setSpeed(500);
	particle->setPosition(cocos2d::Point(x, y));
	this->addChild(particle);
}