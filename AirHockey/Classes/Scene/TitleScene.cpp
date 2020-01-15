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

	/// �z�X�g�V�[���̃{�^������(�΃{�^��)
	auto hostItem = MenuItemImage::create("host.png", "host2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, HostScene::createScene(), Color3B::WHITE));
	});
	hostItem->setPosition(origin.x + visibleSize.width / 2,
						  origin.y + visibleSize.height / 2 + hostItem->getContentSize().height);
	auto hostMenu = Menu::create(hostItem, 0);
	hostMenu->setName("hostMenu");
	hostMenu->setPosition(Vec2::ZERO);
	this->addChild(hostMenu, static_cast<int>(LayerNum::FRONT));

	/// �Q�[���V�[���̃{�^������(�{�^��)
	auto guestItem = MenuItemImage::create("guest.png", "guest2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GuestScene::createScene(), Color3B::WHITE));
	});
	guestItem->setPosition(origin.x + visibleSize.width / 2,
						   origin.y + visibleSize.height / 2 - hostItem->getContentSize().height);
	auto guestMenu = Menu::create(guestItem, 0);
	guestMenu->setName("guestMenu");
	guestMenu->setPosition(Vec2::ZERO);
	this->addChild(guestMenu, static_cast<int>(LayerNum::FRONT));

	auto label = Label::create("Title", "Arial", 60);
	label->setPosition(Vec2::ZERO);
	this->addChild(label);

	// 1�ڰт��Ƃ�update��
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
