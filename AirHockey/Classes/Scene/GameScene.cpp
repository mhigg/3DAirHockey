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
#include "../Obj/StageWall.h"
#include "../Obj/Player.h"
#include "../Effekseer/EffectMng.h"

#include "../Manager/GameManager.h"
#include "../Manager/CCAudioMng.h"
#include "../Manager/AppInfo.h"

#include "../ConsoleOut.h"

USING_NS_CC;

GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{
	// ｼｰﾝの作成
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

	// 消失点の初期化
	lpPointWithDepth.ResetVanishingPoint();

	// 画面解像度の取得
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 座標の取得
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 右上のｼｬｯﾄﾀﾞｳﾝﾎﾞﾀﾝの作成
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

	/// シーン遷移用のボタンの作成
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

	/// ゲーム管理者の生成
	auto gameLayer = Layer::create();
	auto gameMng = GameManager::createGameMng();
	gameMng->GeneratePlayer(lpAppInfo.isHost());
	gameLayer->setName("GameLayer");
	gameLayer->addChild(gameMng);
	this->addChild(gameLayer, static_cast<int>(LayerNum::GAME));

	// ﾌｨｰﾙﾄﾞ用ﾚｲﾔｰ
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

	///// ここの直値を後ほど修正しておく　◆
	// ｽﾃｰｼﾞの壁作成
	// 奥行の最大値
	float maxDepth = gameMng->GetMaxDepth();
	// 壁の最大数
	int wallMaxNum = gameMng->GetDepths().size();
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// 奥行の作成
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	/// 仮の初期化 ◆
	zdepth[0] = 0.f;
	// ﾌｨｰﾙﾄﾞの基本サイズ
	Point wallSize = visibleSize;
	// ﾌｨｰﾙﾄﾞ用ｽﾌﾟﾗｲﾄの作成
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
		// 最後の一枚は黒で描画
		else
		{
			color = Color3B(0, 0, 0);
		}
		//auto stageWall = new StageWall({ 0, 0 }, zdepth[k], wallSize, color);
		auto stageWall = new StageWall(zdepth[k], k);
		// ｽﾃｰｼﾞﾚｲﾔｰに追加
		stageLayer->addChild(stageWall,0,"Wall" + std::to_string(k));
	}

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	/// UI用のレイヤーを作成している
	Layer* UILayer = Layer::create();
	UILayer->setName("UI");

	/// スコアの画像を生成している
	Sprite* score[2];
	Color3B color;
	Vec2 pos;
	for (int i = 0; i < sizeof(score) / sizeof(score[0]); ++i)
	{
		color = (i == 0 ? Color3B(135, 175, 250) : Color3B(250, 135, 135));
		pos = (i == 0 ? Vec2(visibleSize.width / 3, visibleSize.height / 2)
			: Vec2(visibleSize.width * 2 / 3, visibleSize.height / 2));

		score[i] = Sprite::create("image/number.png");
		score[i]->setColor(color);
		score[i]->setPosition(pos);
		score[i]->setVisible(false);
		score[i]->setName("score" + std::to_string(i + 1));

		score[i]->setTextureRect(Rect(0, 0, 100, 100));
		UILayer->addChild(score[i]);
	}

	Sprite* cntDown = Sprite::create("image/number.png");
	Sprite* start = Sprite::create("image/start.png");
	cntDown->setName("cntDown");
	start->setName("start");

	cntDown->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
	cntDown->setVisible(false);
	start->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 4));
	start->setVisible(false);

	UILayer->addChild(cntDown);
	UILayer->addChild(start);

	/// UIの登録をしている
	this->addChild(UILayer, static_cast<int>(LayerNum::FRONT));

	/// 仮のSE再生
	CCAudioMng::GetInstance().RegistBank("SE/ball.ckb", "ball");
	CCAudioMng::GetInstance().RegistBankSE("ball", "curve", "curve");
	CCAudioMng::GetInstance().RegistBankSE("ball", "hit", "hit");
	CCAudioMng::GetInstance().RegistBankSE("ball", "wallHit", "wallHit");

	CCAudioMng::GetInstance().RegistBank("SE/UI.ckb", "UI");
	CCAudioMng::GetInstance().RegistBankSE("UI", "cntDown", "cntDown");
	CCAudioMng::GetInstance().RegistBankSE("UI", "start", "start");
	CCAudioMng::GetInstance().RegistBankSE("UI", "score", "score");

	// 消失点用
	_vPoint = _vel = Vec2::ZERO;

	/// シーン名を付けた
	this->setName("GameScene");
	
	return true;
}

void GameScene::ChangeScene(cocos2d::Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, ResultScene::createScene(), Color3B::WHITE));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
//	networkLogic->disconnect();
	// ｼｬｯﾄﾀﾞｳﾝﾎﾞﾀﾝを押したら閉じる用にする
	Director::getInstance()->end();
}

void GameScene::update(float dt)
{
	CCAudioMng::GetInstance().Update();
	// ｴﾌｪｸﾄの要素がある場合
	if (lpEffectMng.GetManager().empty() == false)
	{
		for (auto data : lpEffectMng.GetManager())
		{
			// Effekseerの更新
			data.second->update();
		}
	}

	/*auto gameMng = getChildByName("GameLayer")->getChildByName("GameManager");
	auto centerPos = gameMng->getChildByName("player1")->getPosition() + gameMng->getChildByName("player2")->getPosition();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto Clamp = [](const float& vel, const float& speed)
	{
		return fmin(3.f, fmax(-3.f, (vel + speed)));
	};

	/// 現状、仮の消失点の更新をしている
	auto distance = (centerPos/2 - _vPoint).getNormalized();
	_vel = Vec2(Clamp(_vel.x, distance.x * 0.2f), Clamp(_vel.y, distance.y * 0.2f));
	_vPoint += _vel;

	lpPointWithDepth.SetVanishingPoint(-_vPoint + Vec2(visibleSize.width / 2, visibleSize.height / 2));*/
}

void GameScene::visit(cocos2d::Renderer * renderer, const cocos2d::Mat4 & parentTransform, uint32_t parentFlags)
{
	// Effekseerの描画設定
	// ｴﾌｪｸﾄの要素がある場合
	if (lpEffectMng.GetManager().empty() == false)
	{
		for (auto data : lpEffectMng.GetManager())
		{
			data.second->begin(renderer, _globalZOrder);
			cocos2d::Scene::visit(renderer, parentTransform, parentFlags);
			data.second->end(renderer, _globalZOrder);
		}
	}
	// ない場合
	else
	{
		cocos2d::Scene::visit(renderer, parentTransform, parentFlags);
	}
}
