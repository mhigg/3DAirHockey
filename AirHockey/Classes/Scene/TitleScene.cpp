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

	// ��ʉ𑜓x�̎擾
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ���W�̎擾
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// �E��̼����޳����݂̍쐬
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

	// 1�ڰт��Ƃ�update��
	this->scheduleUpdate();

	return true;
}

void TitleScene::update(float dt)
{
	Director::getInstance()->end();
}

void TitleScene::ChangeScene(cocos2d::Ref * ref)
{
	/// �z�X�g�V�[���֑J�ڂ���悤�ɂ��Ă���
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, HostScene::createScene(), Color3B::WHITE));
}

void TitleScene::menuCloseCallback(cocos2d::Ref * pSender)
{
}
