﻿#include "Ball.h"
#include "BallAfter.h"
#include "StageWall.h"
#include "../Obj/Player.h"
#include "../Manager/GameManager.h"
#include "../Manager/AnimMng.h"

#include "../Manager/AppInfo.h"
#include "../Manager/CCAudioMng.h"
#include "Collision.h"

USING_NS_CC;
#define M_PI (3.1416)

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

void Ball::ResetPosition(const cocos2d::Vec3 & pos)
{
	_localPos = pos;

	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));
}

std::tuple<bool, bool, bool> Ball::GetIsReverse() const
{
	return _isReverse;
}

bool Ball::Init(void)
{
	// 初期座標
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,_wallDepth[4] };			/// とりあえず仮の位置で設定している　◆

	// 半径(画像の大きさ/2 - 余白)
	_diameter = 150;

	// 最大角度
	int angle = 180;
	// 分割数
	int dNum = 4;
	// 一つ当たりの角度
	float oAngle = angle / dNum;
	for (int x = 0; x < dNum; x++)
	{
		auto *cirlce = DrawNode::create();
		// 中心座標、半径、角度、頂点数、中心に向かう線を描画するか、倍率x、倍率y
		cirlce->drawCircle(Vec2::ZERO, _diameter / 2, 0, 12, false, Color4F::WHITE);
		// 回転
		cirlce->setRotation3D(Vec3{ oAngle*x,0,0 });

		this->addChild(cirlce);
	}
	for (int y = 1; y < dNum; y++)
	{
		auto *cirlce = DrawNode::create();
		// 中心座標、半径、角度、頂点数、中心に向かう線を描画するか、倍率x、倍率y
		cirlce->drawCircle(Vec2::ZERO, _diameter / 2, 0, 12, false, Color4F::WHITE);
		// 回転
		cirlce->setRotation3D(Vec3{ 0, oAngle*y,0 });

		this->addChild(cirlce);
	}

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

int Ball::IsHitAnchor(const Node* pl)
{
	Player* player	= (Player*)pl;
	Size size		= Size(_diameter, _diameter);		// ボールの大きさ
	Vec2 distance	= { 500, 500 };						// ボールからプレイヤーまでの距離()
	int ancType		= -1;								// 当たった場所の取得用
	bool col		= false;							// 当たったかの判定

	for (auto plAnchor : player->getChildren())
	{
		/// ボールとアンカーポイントが当たった時に入る処理
		if (lpCollision.HitCollision2D(Vec2(_localPos.x, _localPos.y), size,
			player->GetAnchorPos(plAnchor->getName()), plAnchor->getContentSize()))
		{
			col = true;
			/// ボールからアンカーポイントの距離が、前回求めた距離より近い時に入る
			if (abs(player->GetAnchorPos(plAnchor->getName()).x - _localPos.x) < distance.x
			&&  abs(player->GetAnchorPos(plAnchor->getName()).y - _localPos.y) < distance.y)
			{
				/// ボールからプレイヤー距離を更新している
				distance = Vec2(abs(player->GetAnchorPos(plAnchor->getName()).x - _localPos.x),
								abs(player->GetAnchorPos(plAnchor->getName()).y - _localPos.y));

				/// プレイヤーの当たった位置の取得
				ancType  = plAnchor->getTag();
			}
		}
	}
	return ancType;
}

void Ball::ChangeIsReverse()
{
	/// ゲームマネージャーの取得
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");

	/// プレイヤー情報の取得
	Player* players[2];
	for (int i = 0; i < sizeof(players) / sizeof(players[0]); ++i)
	{
		players[i] = (Player*)gameMng->getChildByName("player" + std::to_string(i + 1));
	}

	/// 速度の反転用(X)
	if (_localPos.x - _diameter / 2 < -gameMng->GetMovingRange().x && !std::get<0>(_isReverse))
	{
		_ballState = State::NORMAL;

		/// 奥行の割合を求めている
		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);
		CCAudioMng::GetInstance().CkPlaySE("wallHit", rate);

		/// 右方向に反転させる
		std::get<0>(_isReverse) = true;
	}
	else if (_localPos.x + _diameter / 2 > gameMng->GetMovingRange().x && std::get<0>(_isReverse))
	{
		_ballState = State::NORMAL;

		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);
		CCAudioMng::GetInstance().CkPlaySE("wallHit", rate);

		/// 左方向に反転させる
		std::get<0>(_isReverse) = false;
	}
	else {}

	/// 速度の反転用(Y)
	if (_localPos.y - _diameter / 2 < -gameMng->GetMovingRange().y && !std::get<1>(_isReverse))
	{
		_ballState = State::NORMAL;

		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);
		CCAudioMng::GetInstance().CkPlaySE("wallHit", rate);

		/// 上方向に反転させる
		std::get<1>(_isReverse) = true;
	}
	else if (_localPos.y + _diameter / 2 > gameMng->GetMovingRange().y && std::get<1>(_isReverse))
	{
		_ballState = State::NORMAL;

		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);
		CCAudioMng::GetInstance().CkPlaySE("wallHit", rate);

		/// 下方向に反転させる
		std::get<1>(_isReverse) = false;
	}
	else {}

	/// ボールがゴール地点を超えたかの判定を取っている
	if (_localPos.z > players[1]->GetDepth() && std::get<2>(_isReverse))
	{
		/// 当たったアンカーポイントの取得
		int ancType = IsHitAnchor(players[1]);

		/// ボールがプレイヤーに当たった時に入る
		if (ancType >= 0)
		{
			/// ボールと当たった時の画像に変更する
			players[1]->ChangeImage(ancType);

			/// 残像の描画位置を変更する
			auto ballAfter = gameMng->getChildByName("ballAfter");
			ballAfter->setLocalZOrder(static_cast<int>(SpriteNum::SHADOW));

			/// プレイヤーを動かしながらボールを当てた時、カーブを行う。
			ChangeMoving(players[1]);

			/// 手前から奥の方向に反転する
			std::get<2>(_isReverse) = false;
		}
		else
		{
			/// ゴール地点を超えていた時に入る
			if (_localPos.z > players[1]->GetDepth() + _diameter / 2)
			{
				/// 1Pの得点を加算する
				gameMng->TransitionScore(true);
			}
		}
	}
	else if (_localPos.z <= players[0]->GetDepth() && !std::get<2>(_isReverse))
	{
		int ancType = IsHitAnchor(players[0]);

		if (ancType >= 0)
		{
			players[0]->ChangeImage(ancType);

			auto ballAfter = gameMng->getChildByName("ballAfter");
			ballAfter->setLocalZOrder(static_cast<int>(SpriteNum::BALL));

			ChangeMoving(players[0]);

			/// 奥から手前の方向に反転する
			std::get<2>(_isReverse) = true;
		}
		else
		{
			if (_localPos.z <= players[0]->GetDepth() - _diameter / 2)
			{
				gameMng->TransitionScore(false);
			}
		}
	}
	else {}

}

void Ball::ChangeMoving(const Node* pl)
{
	/// ゲームマネージャーの情報取得
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");

	/// プレイヤーの情報取得
	auto player = (Player*)pl;

	/// ボールの状態を変える処理
	if (abs(player->GetMoveDistance().x) >= 20 &&
		abs(player->GetMoveDistance().y) >= 20)
	{
		/// 奥行の割合を求めている
		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);
		CCAudioMng::GetInstance().CkPlaySE("curve", rate);

		/// ボールの状態をカーブに変更する
		_ballState = State::CURVE;
		_traject->CalBezierPoint(player->GetMoveDistance().getNormalized());
	}
	else
	{
		/// 奥行の割合を求めている
		_traject->ChangeVel(player->GetMoveDistance().getNormalized());
		float rate = 1.f - (_localPos.z / _wallDepth[gameMng->GetDepths().size() - 1]);

		/// ボールの状態を通常に変更する
		CCAudioMng::GetInstance().CkPlaySE("hit", rate);
		_ballState = State::NORMAL;

		/// 速度の設定をしている
		Vec3 vel = _traject->GetVel(_ballState);
		std::get<0>(_isReverse) = (vel.x >= 0.f ? true : false);
		std::get<1>(_isReverse) = (vel.y >= 0.f ? true : false);
	}
}

void Ball::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// ゲームシーン以外の時は処理に入らないようにする
		return;
	}
	
	/// ゲームマネージャーの情報取得
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");

	/// ゲーム中の時に入る
	if (gameMng->IsGame())
	{
		/// ボールの移動方向を反転させる処理
		ChangeIsReverse();

		// 1ﾌﾚｰﾑ前の座標(ｱﾆﾒｰｼｮﾝの向き用)
		Vec2 oldPos = { _localPos.x,_localPos.y };

		/// ボールの座標更新
		_localPos += _traject->GetVel(_ballState);

		// ｱﾆﾒｰｼｮﾝの向き
		float angle = atan2(_localPos.y - oldPos.y, _localPos.x - oldPos.x) * 180 / M_PI;
		setRotation(90 + angle);

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
		/// 残像の座標を更新している
		auto ballAfter = (BallAfter*)gameMng->getChildByName("ballAfter");
		ballAfter->Update(_localPos);
	}
	else
	{
		/// ゲーム中でないとき、ボールの状態をリセットする
		_ballState = State::NORMAL;
		_traject->ResetVel();
	}

	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

	// 移動の更新
	float depth;
	if (lpAppInfo.isHost())
	{
		depth = _localPos.z;
	}
	else
	{
		depth = _wallDepth[_wallDepth.size() - 1] - _localPos.z;
	}
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(depth));

}