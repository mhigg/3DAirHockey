#include "TitleScene.h"
#include "GameScene.h"
#include "../Obj/SpriteAffectDepth.h"
#include "../Obj/StageWall.h"
#include "../Obj/Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Manager/PointWithDepth.h"

#include "Manager/AppInfo.h"
#include "NetworkLogic.h"
#include "../ConsoleOut.h"

static const EG_CHAR* appID1 = L"91ccb37c-1396-43af-bbbf-46a4124935a5";
static const EG_CHAR* appID2 = L"b1723cd8-6b7c-4d52-989c-702c2848d8e8";
static const EG_CHAR* appID3 = L"ac500b19-8cfa-47b5-9781-4d9d438496e4";

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

	// 消失点の初期化
	lpPointWithDepth.ResetVanishingPoint();
	_isGyro = true;
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
	auto hostItem = MenuItemImage::create("image/button/host.png", "image/button/host2.png",[&](Ref* ref)
	{
		/// 1Pの設定をしている。
		lpAppInfo.isHost(true);
		lpAppInfo.appID(appID1);	//←画面で選べるようにする
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(_isGyro), Color3B::WHITE));
	});
	hostItem->setPosition(origin.x + visibleSize.width / 3,
						  origin.y + visibleSize.height / 2 - hostItem->getContentSize().height * 1.5f);
	auto hostMenu = Menu::create(hostItem, 0);
	hostMenu->setName("hostMenu");
	hostMenu->setPosition(Vec2::ZERO);


	this->addChild(hostMenu, static_cast<int>(LayerNum::FRONT));
	
	/// ゲストシーンのボタン生成
	auto guestItem = MenuItemImage::create("image/button/guest.png", "image/button/guest2.png",[&](Ref* ref)
	{
		///  2Pの設定をしている
		lpAppInfo.isHost(false);
		lpAppInfo.appID(appID1);	//←画面で選べるようにする
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::createScene(_isGyro), Color3B::WHITE));
	});
	guestItem->setPosition((origin.x + visibleSize.width / 3) * 2,
						    origin.y + visibleSize.height / 2 - guestItem->getContentSize().height * 1.5f);
	auto guestMenu = Menu::create(guestItem, 0);
	guestMenu->setName("guestMenu");
	guestMenu->setPosition(Vec2::ZERO);


	/// ボタンの追加
	this->addChild(guestMenu, static_cast<int>(LayerNum::FRONT));
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	/// Android操作切り替えボタン生成
	auto gyroItem = MenuItemImage::create("image/button/gyro.png", "image/button/gyro2.png", [&](Ref* ref)
	{
		TitleScene::GyroButton(ref);
	});
	gyroItem->setPosition((origin.x + visibleSize.width / 2),
		origin.y + visibleSize.height / 2 - gyroItem->getContentSize().height / 2);
	gyroItem->setScale(0.7f);
	auto gyroMenu = Menu::create(gyroItem, 0);
	gyroMenu->setName("gyroMenu");
	gyroMenu->setPosition(Vec2::ZERO);

	auto touchItem = MenuItemImage::create("image/button/touch.png", "image/button/touch2.png", [&](Ref* ref)
	{
		TitleScene::GyroButton(ref);
	});
	touchItem->setPosition((origin.x + visibleSize.width / 2),
		origin.y + visibleSize.height / 2 - gyroItem->getContentSize().height / 2);
	touchItem->setScale(0.7f);
	auto touchMenu = Menu::create(touchItem, 0);
	touchMenu->setName("touchMenu");
	touchMenu->setPosition(Vec2::ZERO);
	/// ボタンテキストの生成
	//_androidLabel = Label::create("Gyro", "Arial", 40);
	//_androidLabel->setColor(Color3B::BLACK);
	//_androidLabel->setPosition((origin.x + visibleSize.width / 2),
	//	origin.y + visibleSize.height / 2 - gyroItem->getContentSize().height / 2);

	/// ボタンの追加
	this->addChild(gyroMenu, static_cast<int>(LayerNum::FRONT));
	this->addChild(touchMenu, static_cast<int>(LayerNum::FRONT));
	touchMenu->setVisible(false);
	//this->addChild(_androidLabel, static_cast<int>(LayerNum::FRONT));
#endif

	// ﾌｨｰﾙﾄﾞ用ﾚｲﾔｰ
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

	///// ここの直値を後ほど修正しておく　◆
	// ｽﾃｰｼﾞの壁作成
	// 奥行の最大値
	float maxDepth = 1000.f;
	// 壁の最大数
	int wallMaxNum = 10;
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	std::vector<float>zdepth;
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
		auto stageWall = new StageWall(zdepth[k], k, false);
		// ｽﾃｰｼﾞﾚｲﾔｰに追加
		stageLayer->addChild(stageWall, 0, "Wall" + std::to_string(k));
	}
	// titleLogoLayer
	auto logoLayer = Layer::create();
	this->addChild(logoLayer, 5);
	// ﾀｲﾄﾙ文字の表示
	for (float k = 100; k >= 0; k -= 5)
	{
		auto titleLogo = new SpriteAffectDepth(Vec2{ 0,0 }, k, "image/title.png");
		logoLayer->addChild(titleLogo);

	}
	this->setName("TitleScene");

	// ﾌﾟﾚｲﾔｰ作成
	Player* player = new Player(true, zdepth[1]);
	player->setPosition((origin.x + visibleSize.width),origin.y + visibleSize.height / 2);
	player->setName("Player");
	stageLayer->addChild(player);

	/*auto startLogo = Sprite::create("image/tap_to_start.png");
	startLogo->setPosition(Vec2{ visibleSize.width / 2,visibleSize.height / 4 });
	startLogo->setGlobalZOrder(1);
	this->addChild(startLogo);*/

	// 1ﾌﾚｰﾑごとにupdateを
	this->scheduleUpdate();

	return true;
}

void TitleScene::update(float dt)
{
	/// 画面サイズの取得
	//cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	//cocos2d::Vec2 pos = _oprtState->GetPoint();

	//// 入力情報の更新
	//_oprtState->Update();

	//lpPointWithDepth.SetVanishingPoint(pos);
}

void TitleScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}

void TitleScene::GyroButton(cocos2d::Ref * ref)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_isGyro = _isGyro == true ? false : true;
	if (_isGyro == true)
	{
		this->getChildByName("gyroMenu")->setVisible(true);
		this->getChildByName("touchMenu")->setVisible(false);
//		_androidLabel->setString("Gyro");
	}
	else
	{
		this->getChildByName("gyroMenu")->setVisible(false);
		this->getChildByName("touchMenu")->setVisible(true);
//		_androidLabel->setString("Touch");
	}
	auto pl = (Player*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer")->getChildByName("Player");
	pl->GyroSet(_isGyro);
#endif

}
