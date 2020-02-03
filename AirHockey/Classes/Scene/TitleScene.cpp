#include "TitleScene.h"
#include "HostScene.h"
#include "GuestScene.h"
#include "../Obj/SpriteAffectDepth.h"
#include "../Obj/StageWall.h"
#include "../Obj/Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Manager/PointWithDepth.h"

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
	auto hostItem = MenuItemImage::create("host.png", "host2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, HostScene::createScene(), Color3B::WHITE));
	});
	hostItem->setPosition(origin.x + visibleSize.width / 3,
						  origin.y + visibleSize.height / 2 - hostItem->getContentSize().height);
	auto hostMenu = Menu::create(hostItem, 0);
	hostMenu->setName("hostMenu");
	hostMenu->setPosition(Vec2::ZERO);

	/// ボタンテキストの生成
	auto hostLabel = Label::create("Host", "Arial", 50);
	hostLabel->setColor(Color3B::BLACK);
	hostLabel->setPosition(origin.x + visibleSize.width / 3,
						   origin.y + visibleSize.height / 2 - hostItem->getContentSize().height);
	this->addChild(hostMenu, static_cast<int>(LayerNum::FRONT));
	this->addChild(hostLabel, static_cast<int>(LayerNum::FRONT));

	/// ゲストシーンのボタン生成
	auto guestItem = MenuItemImage::create("guest.png", "guest2.png",[&](Ref* ref)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GuestScene::createScene(), Color3B::WHITE));
	});
	guestItem->setPosition((origin.x + visibleSize.width / 3) * 2,
						   origin.y + visibleSize.height / 2 - guestItem->getContentSize().height);
	auto guestMenu = Menu::create(guestItem, 0);
	guestMenu->setName("guestMenu");
	guestMenu->setPosition(Vec2::ZERO);
	/// ボタンテキストの生成
	auto guestLabel = Label::create("Guest", "Arial", 50);
	guestLabel->setColor(Color3B::BLACK);
	guestLabel->setPosition( (origin.x + visibleSize.width / 3) * 2 ,
							origin.y + visibleSize.height / 2 - guestItem->getContentSize().height);

	/// ボタンの追加
	this->addChild(guestMenu, static_cast<int>(LayerNum::FRONT));
	this->addChild(guestLabel, static_cast<int>(LayerNum::FRONT));

	///// 現在のシーンを表すテキスト
	//auto label = Label::create("Title", "Arial", 60);
	//label->setPosition(Vec2(label->getContentSize().width / 2, 
	//						visibleSize.height - label->getContentSize().height / 2));
	//this->addChild(label);

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
	stageLayer->addChild(player);

	auto startLogo = Sprite::create("image/tap_to_start.png");
	startLogo->setPosition(Vec2{ visibleSize.width / 2,visibleSize.height / 4 });
	startLogo->setGlobalZOrder(1);
	this->addChild(startLogo);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//_oprtState.reset(new MouseCtl(this));
#else
	_oprtState.reset(new Oprt_Touch(this));
#endif

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
