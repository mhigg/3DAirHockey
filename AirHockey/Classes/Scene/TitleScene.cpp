#include "TitleScene.h"

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

	// ‰æ–Ê‰ð‘œ“x‚ÌŽæ“¾
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// À•W‚ÌŽæ“¾
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ‰Eã‚Ì¼¬¯ÄÀÞ³ÝÎÞÀÝ‚Ìì¬
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

	// 1ÌÚ°Ñ‚²‚Æ‚Éupdate‚ð
	this->scheduleUpdate();

	return true;
}

void TitleScene::update(float dt)
{
	Director::getInstance()->end();
}

void TitleScene::menuCloseCallback(cocos2d::Ref * pSender)
{
}
