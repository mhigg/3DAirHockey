#include "TrajectControl.h"
#include "../Manager/GameManager.h"
USING_NS_CC;

TrajectControl::TrajectControl()
{
}

TrajectControl::~TrajectControl()
{
}

cocos2d::Vec3 TrajectControl::GetVel(State& state)
{
	if (state == State::NORMAL)
	{
		/// デフォルトの速度を返す
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
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto player = (Player*)
	auto ball = (Ball*)gameMng->getChildByName("ball");

	/// 一次ベジェの曲線を生成するために必要なもの
	Vec3 start, mid, end;

	start = 

	for (int i = 0; i < gameMng->GetDepth().size(); ++i)
	{

	}
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// 自分のZ座標を取得して、進行方向の一つ先の壁の位置にある制御点のベクトルを
	/// 取得して、速度×ベクトル結果を返すようにする
	return Vec3::ZERO;
}
