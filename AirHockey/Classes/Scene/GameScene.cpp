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
#include "SimpleAudioEngine.h"
#include "Math.h"
#include "../Obj/StageWall.h"
#include "../Obj/Ball.h"
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
	this->addChild(menu);

	// ̨���ޗpڲ԰
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

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
	Point wallSize = { 700,500 };
	// ̨���ޗp���ײĂ̍쐬
	for (int k = 0; k < zdepth.size(); k++)
	{
		auto color = new Color3B(
			255 - (255 * (zdepth[k] / 1000)),
			255 - (255 * (zdepth[k] / 1000)),
			255 - (255 * (zdepth[k] / 1000)));
		auto stageWall = new StageWall({ 0, 0 }, zdepth[k], wallSize, color);
		// �ð��ڲ԰�ɒǉ�
		stageLayer->addChild(stageWall);
	}

	//// �ްٗpڲ԰
	//auto ballLayer = Layer::create();
	//ballLayer->setName("BallLayer");
	//this->addChild(ballLayer);
	//// �ްٍ쐬
	//auto ball = new Ball(zdepth);
	//ballLayer->addChild(ball);
	////ballLayer->addChild(lpEffectMng.EffectInit("effect/Ball.efk", "Ball", 100.0f, 1.0f), 0);

	///// �v���C���[�̐���
	//auto charLayer = Layer::create();
	//auto player = Player::createPlayer(0);
	//charLayer->addChild(player);
	//this->addChild(charLayer, static_cast<int>(LayerNum::PLAYER));

	/// �Q�[���Ǘ��҂̐���
	auto gameLayer = Layer::create();
	auto gameMng   = GameManager::createGameMng();
	gameLayer->setName("gameLayer");
	gameLayer->addChild(gameMng);
	this->addChild(gameLayer, static_cast<int>(LayerNum::GAME));

	
	// 1�ڰт��Ƃ�update��
	this->scheduleUpdate();

	return true;
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

