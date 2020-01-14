#include "GuestScene.h"
#include "GameScene.h"

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

	// ��ʉ𑜓x�̎擾
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ���W�̎擾
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// �E��̼����޳����݂̍쐬
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

	/// �V�[���J�ڗp�̃{�^���̍쐬
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

	// 1�ڰт��Ƃ�update��
	this->scheduleUpdate();

	return true;
}

void GuestScene::update(float dt)
{
	
}

void GuestScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(), Color3B::WHITE));
}

void GuestScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}
