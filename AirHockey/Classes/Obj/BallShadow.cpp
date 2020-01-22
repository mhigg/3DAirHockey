#include "BallShadow.h"
#include "Ball.h"
#include "../Manager/GameManager.h"

USING_NS_CC;

BallShadow::BallShadow(int num)
{
	// 上下左右の影のどれかをenumで保存
	_shadowPlace = static_cast<SHADOW>(num);
	Init();
}

BallShadow::BallShadow()
{
	Init();
}

BallShadow::~BallShadow()
{
}

bool BallShadow::Init(void)
{
	// 画像
	setTexture("image/ball_shadow.png");
	// ｽﾌﾟﾗｲﾄの大きさ
	auto size = getContentSize();

	// 初期座標
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	switch (_shadowPlace)
	{
	case UP:
		_localPos = { 0, visibleSize.height / 2 + size.height / 2,0 };
		break;
	case DOWN:
		_localPos = { 0, -visibleSize.height / 2 - size.height,0 };
		break;
	case LEFT:
		// 画像の回転
		setRotation(90);
		_localPos = { -visibleSize.width / 2 - size.height / 2,0,0 };
		break;
	case RIGHT:
		// 画像の回転
		setRotation(90);
		_localPos = { visibleSize.width / 2 + size.height / 2,0,0 };
		break;
	default:
		_localPos = { 0,0,0 };
		break;
	};

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

	return true;
}

void BallShadow::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// ゲームシーン以外の時は処理に入らないようにする
		return;
	}
	/// ゲームマネージャーの取得
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// ボールの取得
	auto ball = (Ball*)gameMng->getChildByName("ball");

	// 画像の拡大縮小用
	float scale = 0;
	// 画面ｻｲｽﾞ
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// 座標と大きさの更新
	switch (_shadowPlace)
	{
	case UP:
		_localPos.x = ball->GetLocalPos().x;
		_localPos.z = ball->GetLocalPos().z;
		scale = (ball->GetLocalPos().y - visibleSize.height / 2) / (visibleSize.height) + 1;
		break;
	case DOWN:
		_localPos.x = ball->GetLocalPos().x;
		_localPos.z = ball->GetLocalPos().z;
		scale = (ball->GetLocalPos().y - visibleSize.height / 2) / (visibleSize.height);
		break;
	case LEFT:
		_localPos.y = ball->GetLocalPos().y;
		_localPos.z = ball->GetLocalPos().z;
		scale = (ball->GetLocalPos().x - visibleSize.width / 2) / (visibleSize.width);
		break;
	case RIGHT:
		_localPos.y = ball->GetLocalPos().y;
		_localPos.z = ball->GetLocalPos().z;
		scale = (ball->GetLocalPos().x - visibleSize.width / 2) / (visibleSize.width) + 1;
		break;
	default:
		_localPos = { 0,0,0 };
		break;
	};

	// 透明度の更新
	setOpacity(255 * abs(scale));

	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z) * abs(scale));
}
