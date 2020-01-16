#include "Ball.h"
#include "StageWall.h"
#include "../Character/Player.h"
#include "../Manager/GameManager.h"
#include "../Manager/AnimMng.h"
#include "Collision.h"

USING_NS_CC;

Ball::Ball(std::vector<float> depth)
{
	_wallDepth = depth;
	Init();
}

Ball::Ball()
{
	Init();
}

Ball::~Ball()
{
}

cocos2d::Vec3 Ball::GetLocalPos(void)
{
	return _localPos;
}


void Ball::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// ゲームシーン以外の時は処理に入らないようにする
		return;
	}
	/// ゲームマネージャーの取得
	auto gameMng  = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// プレイヤーの取得
	auto player = (Player*)gameMng->getChildByName("player");

	if (_localPos.z > _wallDepth[29])
	{
		zReverse = true;
	}
	else if (_localPos.z < player->GetDepth())
	{
		/// プレイヤーとボールの当たり判定
		/// (判定を確認するために一次変数に保存している)
		auto col = Collision::GetInstance().HitCollision2D(this->getPosition(), this->getContentSize(),
														   player->getPosition(), player->getContentSize());
		if (col)
		{
			zReverse = false;
		}
	}
	else{}

	if (!zReverse)
	{
		_localPos.z+=3;
	}
	else
	{
		_localPos.z-=3;
	}
	
	if (_localPos.x - _radius < -gameMng->GetMovingRange().x)
	{
		xReverse = false;
	}
	else if (_localPos.x + _radius > gameMng->GetMovingRange().x)
	{
		xReverse = true;
	}
	else{}

	if (!xReverse)
	{
		_localPos.x += 2;
	}
	else
	{
		_localPos.x -= 2;
	}
	auto debug = gameMng->GetMovingRange();
	if (_localPos.y - _radius < -gameMng->GetMovingRange().y)
	{
		yReverse = false;
	}
	else if (_localPos.y + _radius > gameMng->GetMovingRange().y)
	{
		yReverse = true;
	}
	else{}

	if (!yReverse)
	{
		_localPos.y += 2;
	}
	else
	{
		_localPos.y -= 2;
	}

	// 壁の色更新
	auto director = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
	for (int k = 0; k < _wallDepth.size(); k++)
	{
		auto wall = (StageWall*)director->getChildByName("Wall" + std::to_string(k));

		if (_localPos.z > _wallDepth[k])
		{
			// 変更後のｶﾗｰ
			wall->SetWallColorChangeFlag(true);
		}
		else
		{
			// 通常時のｶﾗｰ
			wall->SetWallColorChangeFlag(false);
		}
	}

	// 座標の更新
	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));


}

bool Ball::Init(void)
{
	// 初期座標
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,_wallDepth[0] };

	// ﾎﾞｰﾙのｱﾆﾒｰｼｮﾝ登録
	lpAnimMng.AddAnimCache("ball", "ball", 15, 0.03f, false);

	// 初期ｱﾆﾒｰｼｮﾝ
	lpAnimMng.SetAnim(this, "ball", false);

	// 半径(画像の大きさ/2 - 余白)
	_radius = 192 / 2 - 32;

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

	return true;
}
