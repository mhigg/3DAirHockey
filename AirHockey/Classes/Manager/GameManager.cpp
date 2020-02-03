#include "../Obj/Player.h"
#include "../Obj/Ball.h"
#include "../Obj/BallAfter.h"
#include "../Obj/Shadow.h"
#include "GameManager.h"
#include "CCAudioMng.h"

USING_NS_CC;

GameManager::GameManager() :
	_maxDepth(1000.f), _wallMax(10),_mimSecond(60),
	_moveRange(1024 / 2, 576 / 2), _playerDepth(1)
{
	Init();
	this->setName("GameManager");
	/// 関数ポインタの中身を初期化している
	_updater = &GameManager::Stay;

	_invCnt = _mimSecond * 4 ;
	this->scheduleUpdate();
}

GameManager::~GameManager()
{
}

GameManager * GameManager::createGameMng()
{
	return GameManager::create();
}

cocos2d::Vec2 GameManager::GetMovingRange() const
{
	return _moveRange;
}

std::vector<float> GameManager::GetDepths() const
{
	return _zdepth;
}

float GameManager::GetMaxDepth(void) const
{
	return _maxDepth;
}

void GameManager::GeneratePlayer(bool isHost)
{
	/// 深度値保存用の一次変数(_wallDepth, _zdepthの0番目は値が壊れているので0でいいかもしれない)　◆
	float depth;
	/// プレイヤーの生成
	Player* player;
	int layer;
	for (int i = 0; i < 2; ++i)
	{
		/// プレイヤーの深度値を設定している(手前：奥)
		depth = (i == 0 ? _zdepth[_playerDepth] : _zdepth[_wallMax - _playerDepth - 1]);
		layer = (i == 0 ? static_cast<int>(SpriteNum::PLAYER) : static_cast<int>(SpriteNum::SHADOW));
		player = new Player(isHost, depth);
		/// プレイヤーの名前を設定している
		//player->setName(isHost ? "HostPlayer" : "GuestPlayer");
		player->setName("player" + std::to_string(i + 1));

		/// プレイヤーの追加
		this->addChild(player, layer);

		isHost ^= 1;	// 一人目と二人目でホストとゲストを分けるため
	}
}

bool GameManager::IsGame() const
{
	return (_updater == &GameManager::Game);
}

void GameManager::TransitionScore()
{
	_updater = &GameManager::Score;
}

void GameManager::Init()
{
	/// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float mag = _maxDepth / (_wallMax * _wallMax);

	/// 深度値保存用の一次変数(_wallDepth, _zdepthの0番目は値が壊れているので0でいいかもしれない)　◆
	float depth;

	for (int x = _wallMax; x > 0; x--)
	{
		depth = x * x * mag;
		depth = _maxDepth - depth;
		_zdepth.emplace_back(depth);
	}
	_zdepth[0] = 0.f;

	/// ボールの生成
	auto ball = new Ball(_zdepth);
	ball->setName("ball");
	this->addChild(ball, static_cast<int>(SpriteNum::BALL));

	/// ボールの影の生成
	for (int k = 0; k < 4; k++)
	{
		auto ballShadow   = new Shadow(k, "ball");
		auto playerShadow = new Shadow(k, "player", "1");
		this->addChild(ballShadow);
		this->addChild(playerShadow);
	}

	/// 残像の生成 (残像の初期位置を修正しておく)　◆
	auto ballAfter = new BallAfter(ball->GetLocalPos());
	ballAfter->setName("ballAfter");
	ballAfter->setVisible(false);
	this->addChild(ballAfter, static_cast<int>(SpriteNum::BALL));

	// GeneratePlayer(true);
}

void GameManager::Stay()
{
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");
	/// UI画像を全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}
	
	Sprite* sp = Sprite::create();
	if (_invCnt < _mimSecond)
	{
		if (!CCAudioMng::GetInstance().IsPlaySE("start"))
		{
			_updater = &GameManager::Game;
			this->getChildByName("ballAfter")->setVisible(true);
			return;
		}
		sp = (Sprite*)UI->getChildByName("start");
		sp->setVisible(true);

	}
	else
	{
		sp = (Sprite*)UI->getChildByName("cntDown");
		sp->setVisible(true);

		/// Y座標の指定位置は現状動かないので、直値で渡している
		Rect rect = Rect(100 * (_invCnt / _mimSecond), 0, 100, 100);
		sp->setTextureRect(rect);

		/// カウントダウンの音を再生する
		if (_invCnt % 60 == 0 && _invCnt > _mimSecond)
		{
			CCAudioMng::GetInstance().CkPlaySE("cntDown");
		}
	}

	/// スタートを知らせる音を再生する
	if (_invCnt == _mimSecond)
	{
		CCAudioMng::GetInstance().CkPlaySE("start");
	}

	--_invCnt;
}

void GameManager::Game()
{
}

void GameManager::Score()
{
}

void GameManager::update(float dt)
{
	/// 関数ポインタの中身の処理を行う
	(this->*_updater)();
}
