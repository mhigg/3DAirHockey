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
#include "../Character/Player.h"
#include "../Effekseer/EffectMng.h"

#include "../Manager/GameManager.h"
#include "../Manager/CCAudioMng.h"

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

	// ﾌｨｰﾙﾄﾞ用ﾚｲﾔｰ
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

	///// ここの直値を後ほど修正しておく　◆
	// ｽﾃｰｼﾞの壁作成
	// 奥行の最大値
	float maxDepth = 1000;
	// 壁の最大数
	int wallMaxNum = 30;
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// 奥行の作成
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
		zdepth.emplace_back(depth);
	}
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
		auto stageWall = new StageWall({ 0, 0 }, zdepth[k], wallSize, color);
		// ｽﾃｰｼﾞﾚｲﾔｰに追加
		stageLayer->addChild(stageWall,0,"Wall" +std::to_string(k));
	}

	/// ゲーム管理者の生成
	auto gameLayer = Layer::create();
	auto gameMng   = GameManager::createGameMng();
	gameLayer->setName("GameLayer");
	gameLayer->addChild(gameMng);
	this->addChild(gameLayer, static_cast<int>(LayerNum::GAME));

	/// 現在のシーンを表すテキスト
	auto label = Label::create("Game", "Arial", 60);
	label->setPosition(Vec2(label->getContentSize().width / 2,
							visibleSize.height - label->getContentSize().height / 2));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	_swallowsTouches = true;
	// シングルタップリスナーを用意する
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(_swallowsTouches);
 
	// 各イベントの割り当て
	listener->onTouchBegan     = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved     = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded     = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
 
	// イベントディスパッチャにシングルタップ用リスナーを追加する
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	// Photonネットワーククラスのインスタンスを作成
	networkLogic = new NetworkLogic(&ConsoleOut::get());

	// 毎フレームでupdateを実行させる
	this->schedule(schedule_selector(GameScene::update));

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	/// 仮のBGM再生
	CCAudioMng::GetInstance().RegistStreamBGM("BGM/bgm.cks", "bgm");
	CCAudioMng::GetInstance().CkPlayBGM("bgm");

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
	networkLogic->disconnect();
	// ｼｬｯﾄﾀﾞｳﾝﾎﾞﾀﾝを押したら閉じる用にする
	Director::getInstance()->end();
}

void GameScene::update(float dt)
{
	networkLogic->run();

	switch (networkLogic->getState()) {
	case STATE_CONNECTED:
	case STATE_LEFT:
		// ルームが存在すればジョイン、なければ作成する
		if (networkLogic->isRoomExists()) {
			networkLogic->setLastInput(INPUT_2);
		}
		else {
			networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// 接続が切れたら再度接続
		networkLogic->connect();
		break;
	case STATE_CONNECTING:
	case STATE_JOINING:
	case STATE_JOINED:
	case STATE_LEAVING:
	case STATE_DISCONNECTING:
	default:
		break;
	}

	while (!networkLogic->eventQueue.empty()) {
		std::array<float, 3>arr = networkLogic->eventQueue.front();
		networkLogic->eventQueue.pop();

		int playerNr = static_cast<int>(arr[0]);
		float x = arr[1];
		float y = arr[2];
		CCLOG("%d, %f, %f", playerNr, x, y);

		this->addParticle(playerNr, x, y);
	}

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


bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	if (networkLogic->playerNr) {
		this->addParticle(networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);

		// イベント（タッチ座標）を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, touch->getLocation().x);
		eventContent->put<int, float>(2, touch->getLocation().y);
		networkLogic->sendEvent(1, *eventContent);
	}

	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void GameScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void GameScene::addParticle(int playerNr, float x, float y)
{
	ParticleSystem* particle;
	switch (playerNr) {
	case 1:
		particle = ParticleFire::create();
		break;
	case 2:
		particle = ParticleSmoke::create();
		break;
	case 3:
		particle = ParticleFlower::create();
		break;
	default:
		particle = ParticleSun::create();
		break;
	}
	particle->setDuration(0.1);
	particle->setSpeed(500);
	particle->setPosition(cocos2d::Point(x, y));
	this->addChild(particle);
}