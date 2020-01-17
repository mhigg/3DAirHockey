#include "TrajectControl.h"
#include "../Manager/GameManager.h"
#include "../Obj/Ball.h"
#include "../Character/Player.h"

USING_NS_CC;

TrajectControl::TrajectControl() : _speed(6.f, 6.f,6.f)
{
}

TrajectControl::~TrajectControl()
{
}

bool TrajectControl::CalBezierPoint()
{
	if (Director::getInstance()->getRunningScene() == nullptr)
	{
		/// ゲームシーン以外の時は処理に入らないようにする
		return false;
	}

	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return false;
	}

	/// 端点の生成に必要なものを取得している
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto player		= (Player*)gameMng->getChildByName("player");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// 一次ベジェの曲線を生成するために必要なもの
	Vec3 start, mid, end;
	float a, b;

	start = Vec3(player->getPositionX(), player->getPositionY(), player->GetDepth());
	mid = Vec3(400, 300, gameMng->GetDepth()[8]);
	end = Vec3(-500, -250, gameMng->GetDepth()[29]);				/// 仮の設定　◆

	/// ベジェ曲線の計算を使い、端点の位置を求めている
	/// 深度値によって、登録する順番を変えなければならない　◆
	for (int i = 0; i < _points.size(); ++i)
	{
		b = (float)i / _points.size();
		a = 1.f - b;

		/// 端点の登録をしている
		_points[i].x = (a * a * start.x) + (2 * a * b * mid.x) + (b * b * end.x);
		_points[i].y = (a * a * start.y) + (2 * a * b * mid.y) + (b * b * end.y);
		_points[i].z = (a * a * start.z) + (2 * a * b * mid.z) + (b * b * end.z);
	}

	return true;
}

cocos2d::Vec3 TrajectControl::GetVel(const State& state)
{
	if (state == State::NORMAL)
	{
		/// デフォルトの速度を返す
		return CalNormalVel();
	}
	else if (state == State::CURVE)
	{
		/// 計算した速度を返す
		return CalCurveVel();
	}
	return Vec3::ZERO;
}

cocos2d::Vec3 TrajectControl::CalNormalVel()
{
	/// ボールの情報を取得している
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// 速度を反転させるかのフラグを取得している
	auto isReverse	= ball->GetIsReverse();
	cocos2d::Vec3 vel;

	/// 速度の設定
	vel.x = (!std::get<0>(isReverse) ? _speed.x : -_speed.x);
	vel.y = (!std::get<1>(isReverse) ? _speed.y : -_speed.y);
	vel.z = (!std::get<2>(isReverse) ? _speed.z : -_speed.z);

	return vel;
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// 自分のZ座標を取得して、進行方向の一つ先の壁の位置にある制御点のベクトルを
	/// 取得して、速度×ベクトルの計算結果を返すようにする
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng  = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball	  = (Ball*)gameMng->getChildByName("ball");
	
	// Z軸の進行方向を取得している	◆
	auto zReverse = std::get<2>(ball->GetIsReverse());
	Vec3 vec;
	if (!zReverse)
	{
		for (int i = 0; i < _points.size(); ++i)
		{
			if (_points[i].z > ball->GetLocalPos().z)
			{
				/// 正規化したベクトルを取得している
				vec = (_points[i] - ball->GetLocalPos());
				vec.normalize();
				return cocos2d::Vec3(_speed.x * vec.x * 2, _speed.y * vec.y * 2, _speed.z * vec.z * 2);
			}
		}
	}
	else
	{
		for (int i = _points.size() - 1; i >= 0; --i)
		{
			if (_points[i].z < ball->GetLocalPos().z)
			{
				/// 正規化したベクトルを取得している
				vec = (_points[i] - ball->GetLocalPos());
				vec.normalize();
				return cocos2d::Vec3(_speed.x * vec.x * 2, _speed.y * vec.y * 2, _speed.z * vec.z * 2);
			}
		}
	}

	/// 速度の修正は後で行うようにする
	float velZ = (!zReverse ? _speed.z : -_speed.z);
	return cocos2d::Vec3(0, 0, velZ);
}
