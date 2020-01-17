#include "TitleScene.h"
#include "HostScene.h"
#include "GuestScene.h"

USING_NS_CC;

TitleScene::~TitleScene()
{

}

cocos2d::Scene * TitleScene::createScene()
{
	return TitleScene::create();
}

bool TitleScene::init()
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
		CC_CALLBACK_1(TitleScene::menuCloseCallback, this));
	auto closeItemVec =
		Vec2{ origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
			  origin.y + visibleSize.height - closeItem->getContentSize().height / 2 };
	closeItem->setPosition(closeItemVec);
	auto menu = Menu::create(closeItem, NULL);
	menu->setName("Menu");
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	/// ホストシーンのボタン生成(緑ボタン)
	auto hostItem = MenuItemImage::create("host.png", "host2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, HostScene::createScene(), Color3B::WHITE));
	});
	hostItem->setPosition(origin.x + visibleSize.width / 2,
						  origin.y + visibleSize.height / 2 + hostItem->getContentSize().height);
	auto hostMenu = Menu::create(hostItem, 0);
	hostMenu->setName("hostMenu");
	hostMenu->setPosition(Vec2::ZERO);

	/// ボタンテキストの生成
	auto hostLabel = Label::create("Host", "Arial", 50);
	hostLabel->setColor(Color3B::BLACK);
	hostLabel->setPosition(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 2 + hostItem->getContentSize().height);
	this->addChild(hostMenu, static_cast<int>(LayerNum::FRONT));
	this->addChild(hostLabel, static_cast<int>(LayerNum::FRONT));

	/// ゲストシーンのボタン生成
	auto guestItem = MenuItemImage::create("guest.png", "guest2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GuestScene::createScene(), Color3B::WHITE));
	});
	guestItem->setPosition(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 2 - guestItem->getContentSize().height);
	auto guestMenu = Menu::create(guestItem, 0);
	guestMenu->setName("guestMenu");
	guestMenu->setPosition(Vec2::ZERO);
	/// ボタンテキストの生成
	auto guestLabel = Label::create("Guest", "Arial", 50);
	guestLabel->setColor(Color3B::BLACK);
	guestLabel->setPosition(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 2 - guestItem->getContentSize().height);

	/// ボタンの追加
	this->addChild(guestMenu, static_cast<int>(LayerNum::FRONT));
	this->addChild(guestLabel, static_cast<int>(LayerNum::FRONT));

	/// 現在のシーンを表すテキスト
	auto label = Label::create("Title", "Arial", 60);
	label->setPosition(Vec2(label->getContentSize().width / 2, 
							visibleSize.height - label->getContentSize().height / 2));
	this->addChild(label);

	this->setName("TitleScene");

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	return true;
}

void TitleScene::update(float dt)
{
	
}

void TitleScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}
