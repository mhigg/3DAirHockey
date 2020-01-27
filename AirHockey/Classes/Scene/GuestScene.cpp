#include "GuestScene.h"
#include "GameScene.h"

#include "Controller/OPRT_Network.h"
#include "../ConsoleOut.h"

static const EG_CHAR* appID1 = L"91ccb37c-1396-43af-bbbf-46a4124935a5";
static const EG_CHAR* appID2 = L"b1723cd8-6b7c-4d52-989c-702c2848d8e8";

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

	inputNetwork.reset(new OPRT_Network(false));

	//_swallowsTouches = true;
	//// シングルタップリスナーを用意する
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(_swallowsTouches);

	//// 各イベントの割り当て
	//listener->onTouchBegan = CC_CALLBACK_2(GuestScene::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(GuestScene::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(GuestScene::onTouchEnded, this);
	//listener->onTouchCancelled = CC_CALLBACK_2(GuestScene::onTouchCancelled, this);

	//// イベントディスパッチャにシングルタップ用リスナーを追加する
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//// Photonネットワーククラスのインスタンスを作成
	//networkLogic = new NetworkLogic(&ConsoleOut::get(), appID1);

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	return true;
}

void GuestScene::update(float dt)
{
	//networkLogic->run();
	//switch (networkLogic->getState()) {
	//case STATE_CONNECTED:
	//case STATE_LEFT:
	//	// ゲスト側で、ルームが存在すればジョイン
	//	if (networkLogic->isRoomExists()) {
	//		networkLogic->setLastInput(INPUT_2);
	//	}
	//	break;
	//case STATE_DISCONNECTED:
	//	// 接続が切れたら再度接続
	//	networkLogic->connect();
	//	break;
	//case STATE_CONNECTING:
	//case STATE_JOINING:
	//case STATE_JOINED:
	//case STATE_LEAVING:
	//case STATE_DISCONNECTING:
	//default:
	//	break;
	//}

	// ﾈｯﾄﾜｰｸ通信での送受信情報の更新
	inputNetwork->Update();

	// ﾃﾞｰﾀの受信と処理
	//while (!networkLogic->eventQueue.empty()) {
	//	std::array<float, 3>arr = networkLogic->eventQueue.front();
	//	networkLogic->eventQueue.pop();

	//	int playerNr = static_cast<int>(arr[0]);
	//	float x = arr[1];
	//	float y = arr[2];
	//	CCLOG("%d, %f, %f", playerNr, x, y);

	auto receivePos = inputNetwork->GetPoint();
	this->addParticle(2, receivePos.x, receivePos.y);
	//}
}

void GuestScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(), Color3B::WHITE));
}

void GuestScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}

//bool GuestScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
//{
//
//	if (networkLogic->playerNr) {
//		this->addParticle(networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);
//
//		// イベント（タッチ座標）を送信
//		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
//		eventContent->put<int, float>(1, touch->getLocation().x);
//		eventContent->put<int, float>(2, touch->getLocation().y);
//		networkLogic->sendEvent(1, eventContent);
//	}
//
//	return true;
//}
//
//void GuestScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
//
//}
//
//void GuestScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
//
//}
//
//void GuestScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
//
//}

void GuestScene::addParticle(int playerNr, float x, float y)
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