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
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

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
	Player* players[2];
	for (int i = 0; i < sizeof(players) / sizeof(players[0]); ++i)
	{
		players[i] = (Player*)gameMng->getChildByName("player" + std::to_string(i + 1));
	}

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

	if (_localPos.z > players[1]->GetDepth())
	{
		bool col;
		Size size = Size(_radius * 2, _radius * 2);
		for (auto plAnchor : players[1]->getChildren())
		{
			/// 2Pとボールの当たり判定
			col = lpCollision.HitCollision2D(Vec2(_localPos.x, _localPos.y), size,
											 players[1]->GetAnchorPos(plAnchor->getName()), plAnchor->getContentSize());
			if (col && !std::get<2>(_isReverse))
			{
				/// ボールと当たった時の画像に変更する
				players[1]->ChangeImage(plAnchor->getTag());

				/// 残像の描画位置を変更する
				auto ballAfter = gameMng->getChildByName("ballAfter");
				ballAfter->setLocalZOrder(static_cast<int>(SpriteNum::SHADOW));

				/// 状態を変更するための処理(デバッグ用)
				_ballState = (State)(rand() % 2);
				if (_ballState == State::CURVE)
				{
					_traject->CalBezierPoint();
				}
				ChangeMoving(players[1]);
				break;
			}
		}
	}
	else if (_localPos.z <= players[0]->GetDepth())
	{
		/// プレイヤーの矩形ごとに当たり判定を取っている
		bool col;
		Size size = Size(_radius * 2, _radius * 2);
		for (auto plAnchor : players[0]->getChildren())
		{
			col = lpCollision.HitCollision2D(Vec2(_localPos.x, _localPos.y), size,
											 players[0]->GetAnchorPos(plAnchor->getName()), plAnchor->getContentSize());
			if (col && std::get<2>(_isReverse))
			{
				/// ボールと当たった時の画像に変更する
				players[0]->ChangeImage(plAnchor->getTag());

				/// 残像の描画位置を変更する
				auto ballAfter = gameMng->getChildByName("ballAfter");
				ballAfter->setLocalZOrder(static_cast<int>(SpriteNum::BALL));
				
				ChangeMoving(players[0]);
				
				std::get<2>(_isReverse) = false;
				break;
			}
		}
	}
	else {}
}

void Ball::ChangeMoving(const Node* pl)
{
	/// プレイヤーの情報取得
	auto player = (Player*)pl;

	/// 状態を変更するための処理(デバッグ用)
	_ballState = (State)(rand() % 2);
	if (_ballState == State::CURVE)
	{
		_traject->CalBezierPoint();
	}
	else  // 現状、ボールの状態は2種類しかないのでif～elseを使った2分岐処理にしている
	{
		/// ボールの跳ね返す方向を切り替えるかの判定
		if (abs(player->GetMoveDistance().x) >= 2)
		{
			std::get<0>(_isReverse) = (player->GetMoveDistance().x < 0 ? true : false);
		}
		if (abs(player->GetMoveDistance().y) >= 2)
		{
			std::get<1>(_isReverse) = (player->GetMoveDistance().y < 0 ? true : false);
		}
	}
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
	auto director = Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
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
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	/// 残像の座標を更新している
	auto gameMng	= Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ballAfter	= (BallAfter*)gameMng->getChildByName("ballAfter");
	ballAfter->Update(_localPos);
}