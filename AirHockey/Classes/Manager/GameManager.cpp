#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "GameManager.h"

/* ヘッダーなどをフォルダー指定する予定なので、後でパス修正を行う　◆ */
USING_NS_CC;

GameManager::GameManager()
{
	/// プレイヤーの追加
	this->addChild(Player::createPlayer(1.0));
	/// ボールの追加
	ballCreate();
	this->setName("gameManager");

	this->scheduleUpdate();
}

GameManager::~GameManager()
{
}

GameManager * GameManager::createGameMng()
{
	return GameManager::create();
}

void GameManager::ballCreate()
{
	/// ボールの生成はとりあえず、コピペ　◆
	std::vector<float> zdepth;
	// ｽﾃｰｼﾞの壁作成
	// 奥行の最大値
	float maxDepth = 1000;
	// 壁の最大数
	int wallMaxNum = 30;
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// 奥行の作成
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	// ﾌｨｰﾙﾄﾞの基本サイズ
	Point wallSize = { 700,500 };

	auto ball = new Ball(zdepth);
	ball->setName("ball");
	this->addChild(ball);
}

void GameManager::update(float dt)
{
}
