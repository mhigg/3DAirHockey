/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameScene.h"
#include "ResultScene.h"
#include "SimpleAudioEngine.h"
#include "Math.h"
#include "../Obj/StageWall.h"
#include "../Character/Player.h"
#include "../Effekseer/EffectMng.h"

#include "../Manager/GameManager.h"

USING_NS_CC;

GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{
	// ��݂̍쐬
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
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
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	auto closeItemVec =
		Vec2{ origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
			  origin.y + visibleSize.height - closeItem->getContentSize().height / 2 };
	closeItem->setPosition(closeItemVec);
	auto menu = Menu::create(closeItem, NULL);
	menu->setName("Menu");
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, static_cast<int>(LayerNum::FRONT));

	/// �V�[���J�ڗp�̃{�^���̍쐬
	auto sceneItem = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(GameScene::ChangeScene, this));
	auto sceneItemVec = Vec2(origin.x + visibleSize.width - sceneItem->getContentSize().width / 2, 
							 sceneItem->getContentSize().height / 2);
	sceneItem->setPosition(sceneItemVec);
	auto sceneMenu = Menu::create(sceneItem, 0);
	sceneMenu->setName("SceneMenu");
	sceneMenu->setPosition(Vec2::ZERO);
	this->addChild(sceneMenu, static_cast<int>(LayerNum::FRONT));

	// ̨���ޗpڲ԰
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

	///// �����̒��l����قǏC�����Ă����@��
	// �ð�ނ̕Ǎ쐬
	// ���s�̍ő�l
	float maxDepth = 1000;
	// �ǂ̍ő吔
	int wallMaxNum = 30;
	// 2���֐��Ŕz�u����̂Ÿ��̂̊J������쐬
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// ���s�̍쐬
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	// ̨���ނ̊�{�T�C�Y
	Point wallSize = visibleSize;
	// ̨���ޗp���ײĂ̍쐬
	for (int k = 0; k < zdepth.size(); k++)
	{
		Color3B color;
		if (k != zdepth.size() - 1)
		{
			color = Color3B(
				255 - (200 * (zdepth[k] / 1000)),
				255 - (200 * (zdepth[k] / 1000)),
				255 - (200 * (zdepth[k] / 1000)));
		}
		// �Ō�̈ꖇ�͍��ŕ`��
		else
		{
			color = Color3B(0, 0, 0);
		}
		auto stageWall = new StageWall({ 0, 0 }, zdepth[k], wallSize, color);
		// �ð��ڲ԰�ɒǉ�
		stageLayer->addChild(stageWall,0,"Wall" +std::to_string(k));
	}

	/// �Q�[���Ǘ��҂̐���
	auto gameLayer = Layer::create();
	auto gameMng   = GameManager::createGameMng();
	gameLayer->setName("GameLayer");
	gameLayer->addChild(gameMng);
	this->addChild(gameLayer, static_cast<int>(LayerNum::GAME));

	/// ���݂̃V�[����\���e�L�X�g
	auto label = Label::create("Game", "Arial", 60);
	label->setPosition(Vec2(label->getContentSize().width / 2,
							visibleSize.height - label->getContentSize().height / 2));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// 1�ڰт��Ƃ�update��
	this->scheduleUpdate();

	/// �V�[������t����
	this->setName("GameScene");

	return true;
}


void GameScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, ResultScene::createScene(), Color3B::WHITE));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	// �����޳����݂������������p�ɂ���
	Director::getInstance()->end();
}

void GameScene::update(float dt)
{
	for (auto data : lpEffectMng.GetManager())
	{
		// Effekseer�̍X�V
		data.second->update();
	}
}

//void GameScene::visit(cocos2d::Renderer * renderer, const cocos2d::Mat4 & parentTransform, uint32_t parentFlags)
//{
//	//// Effekseer�̕`��ݒ�
//	//for (auto data : lpEffectMng.GetManager())
//	//{
//	//	data.second->begin(renderer, _globalZOrder);
//	//	cocos2d::Scene::visit(renderer, parentTransform, parentFlags);
//	//	data.second->end(renderer, _globalZOrder);
//	//}
//}

