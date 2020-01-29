#include "HostScene.h"
#include "GameScene.h"

#include "Manager/GameManager.h"
#include "Manager/AppInfo.h"

#include "NetworkLogic.h"
#include "../ConsoleOut.h"

static const EG_CHAR* appID1 = L"91ccb37c-1396-43af-bbbf-46a4124935a5";
static const EG_CHAR* appID2 = L"b1723cd8-6b7c-4d52-989c-702c2848d8e8";
static const EG_CHAR* appID3 = L"ac500b19-8cfa-47b5-9781-4d9d438496e4";

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

	lpAppInfo.isHost(true);
	lpAppInfo.appID(appID3);	//←画面で選べるようにする

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();
	return true;
}

void HostScene::update(float dt)
{
}

void HostScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(), Color3B::WHITE));
}

void HostScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}
