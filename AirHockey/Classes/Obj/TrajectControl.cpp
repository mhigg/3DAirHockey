#include "TrajectControl.h"
#include "../Manager/GameManager.h"
#include "../Obj/Ball.h"
#include "../Character/Player.h"

USING_NS_CC;

TrajectControl::TrajectControl() : _speed(2.f,2.f,2.f)
{
	_vel = _speed;
}

TrajectControl::~TrajectControl()
{
}

cocos2d::Vec3 TrajectControl::GetVel(State& state)
{
	if (state == State::NORMAL)
	{
		/// デフォルトの速度を返す
		/// ここに速度を反転させるかの処理を追加する
		return _vel;
	}
	else if (state == State::CURVE)
	{
		/// 計算した速度を返す
		return CalCurveVel();
	}
	/// 例外処理
	return _vel;
}

void TrajectControl::CalBezierPoint()
{
	/// 走っているシーンがゲームシーンの時という条件が必要な場合
	/// 判定処理として追加しておく

	/// 端点の生成に必要なものを取得している
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto player		= (Player*)gameMng->getChildByName("player");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// 一次ベジェの曲線を生成するために必要なもの
	Vec3 start, mid, end;
	float a, b;

	start = Vec3(player->getPositionX(), player->getPositionY(), player->GetDepth());
	mid = Vec3(300, 200, gameMng->GetDepth()[13]);
	end = Vec3(200, 50, gameMng->GetDepth()[29]);				/// 仮の設定　◆

	/// ベジェ曲線の計算を使い、端点の位置を求めている
	/// 深度値によって、登録する順番を変えなければならない　◆
	for (int i = 0; i < _points.size(); ++i)
	{
		b = (float)(i / _points.size());
		a = 1.f - b;

		/// 端点の登録をしている
		_points[i].x = (a * a * start.x) + (2 * a * b * mid.x) + (b * b * end.x);
		_points[i].y = (a * a * start.y) + (2 * a * b * mid.y) + (b * b * end.y);
		_points[i].z = (a * a * start.z) + (2 * a * b * mid.z) + (b * b * end.z);
	}
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// 自分のZ座標を取得して、進行方向の一つ先の壁の位置にある制御点のベクトルを
	/// 取得して、速度×ベクトル結果を返すようにする
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng  = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball	  = (Ball*)gameMng->getChildByName("ball");
	/// Z軸の進行方向を確認して、端点を見る位置を変える

	return Vec3::ZERO;
}
