#include "Ball.h"
#include "BallAfter.h"
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

cocos2d::Vec3 Ball::GetLocalPos(void) const
{
	return _localPos;
}

std::tuple<bool, bool, bool> Ball::GetIsReverse() const
{
	return _isReverse;
}

bool Ball::Init(void)
{
	// 初期座標
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,0 };

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

	/// 判定の初期化
	_isReverse = { false, false, false };

	/// ボールの軌道制御用クラスの生成
	_traject.reset(new TrajectControl());

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

	/// ボールの状態の初期化
	_ballState = State::NORMAL;

	return true;
}

void Ball::ChangeIsReverse()
{
	/// ゲームマネージャーの取得
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// プレイヤーの取得
	auto player = (Player*)gameMng->getChildByName("player");

	/// 反転フラグの更新(X)
	if (_localPos.x - _radius < -gameMng->GetMovingRange().x)
	{
		std::get<0>(_isReverse) = false;
	}
	else if (_localPos.x + _radius > gameMng->GetMovingRange().x)
	{
		std::get<0>(_isReverse) = true;
	}
	else {}

	if (_localPos.y - _radius < -gameMng->GetMovingRange().y)
	{
		std::get<1>(_isReverse) = false;
	}
	else if (_localPos.y + _radius > gameMng->GetMovingRange().y)
	{
		std::get<1>(_isReverse) = true;
	}
	else {}

	if (_localPos.z > _wallDepth[29])
	{
		if (!std::get<2>(_isReverse))
		{
			/// 状態を変更するための処理(デバッグ用)
			_ballState = (State)(rand() % 2);
			if (_ballState == State::CURVE)
			{
				_traject->CalBezierPoint();
			}
		}
		std::get<2>(_isReverse) = true;
	}
	else if (_localPos.z <= player->GetDepth())
	{	
		auto col = Collision::GetInstance().HitCollision2D(this->getPosition(), this->getContentSize(),
														   player->getPosition(), player->getContentSize());
		if (col)
		{
			if (std::get<2>(_isReverse))
			{
				/// 状態を変更するための処理(デバッグ用)
				_ballState = (State)(rand() % 2);
				if (_ballState == State::CURVE)
				{
					_traject->CalBezierPoint();
				}
			}
			std::get<2>(_isReverse) = false;
		}
	}
	else {}
}


void Ball::update(float dt)
{
	auto debug = dt;
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// ゲームシーン以外の時は処理に入らないようにする
		return;
	}

	/// 反転フラグの変更を行っている
	ChangeIsReverse();

	_localPos += _traject->GetVel(_ballState);

	// 壁の色更新
	auto director = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
	StageWall* wall;
	for (int k = 0; k < _wallDepth.size(); k++)
	{
		wall = (StageWall*)director->getChildByName("Wall" + std::to_string(k));

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

	/// 残像の座標を更新している
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ballAfter = (BallAfter*)gameMng->getChildByName("ballAfter");
	ballAfter->Update(_localPos);
}