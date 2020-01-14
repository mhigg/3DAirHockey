#include "ResultScene.h"

USING_NS_CC;

ResultScene::~ResultScene()
{
}

cocos2d::Scene * ResultScene::createScene()
{
	return ResultScene::create();
}

bool ResultScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// ��ʉ𑜓x�̎擾
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ���W�̎擾
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// �E��̼����޳����݂̍쐬
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(ResultScene::menuCloseCallback, this));
	auto closeItemVec =
		Vec2{ origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
			  origin.y + visibleSize.height - closeItem->getContentSize().height / 2 };
	closeItem->setPosition(closeItemVec);
	auto menu = Menu::create(closeItem, NULL);
	menu->setName("Menu");
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	// 1�ڰт��Ƃ�update��
	this->scheduleUpdate();
	return true;
}

void ResultScene::update(float dt)
{
}

void ResultScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}
