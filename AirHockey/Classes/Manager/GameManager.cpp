#include "../Obj/Player.h"
#include "../Obj/Ball.h"
#include "../Obj/BallAfter.h"
#include "../Obj/Shadow.h"
#include "GameManager.h"
#include "AppInfo.h"
#include "../Scene/TitleScene.h"
#include "CCAudioMng.h"

USING_NS_CC;

GameManager::GameManager() :
	_maxDepth(1000.f), _wallMax(10),_mimSecond(60),
	_moveRange(1024 / 2, 576 / 2), _playerDepth(1)
{
	Init();
	this->setName("GameManager");
	/// 関数ポインタの中身を初期化している
	_updater = &GameManager::Connect;

	_invCnt = _mimSecond * 4 ;
	_scores[0] = 0;
	_scores[1] = 0;
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

void GameManager::GeneratePlayer(bool isHost, bool setFlag)
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
		player = new Player(isHost, depth, i);
		if (i == 0)
		{
			player->GyroSet(setFlag);
		}
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

void GameManager::TransitionScore(bool isPlayer)
{
	_isPlayer	= isPlayer;
	
	/// プレイヤーのスコアを加算している
	if (_isPlayer)
	{
		++_scores[0];
	}
	else
	{
		++_scores[1];
	}
	this->getChildByName("ballAfter")->setVisible(false);
	_invCnt		= _mimSecond * 4 + 1;
	_updater	= &GameManager::Score;
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
	auto ballAfter = new BallAfter();
	ballAfter->setName("ballAfter");
	ballAfter->setVisible(false);
	this->addChild(ballAfter, static_cast<int>(SpriteNum::BALL));

	// GeneratePlayer(true);
}

void GameManager::Connect()
{
	int peers = lpAppInfo.getJoiningPeer();
	bool isConnection = (lpAppInfo.isHost() ? (peers == 2) : (peers == 1));
	if (isConnection)
	{
		// 接続が完了したら(接続数が２になったら)Stayに移行
		_updater = &GameManager::Stay;
	}
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
		if (_invCnt % 60 == 0&& _invCnt > _mimSecond)
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
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");
	/// UI画像を全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}

	if (_invCnt <= 0)
	{
		bool isGame = true;
		for (auto score : _scores)
		{
			if (score >= 5)
			{
				isGame = false;
				break;
			}
		}

		if (isGame)
		{
			_updater = &GameManager::Stay;
			_invCnt = _mimSecond * 4;

			Ball* ball = (Ball*)this->getChildByName("ball");
			BallAfter* ballAfter = (BallAfter*)this->getChildByName("ballAfter");
			ballAfter->ResetPosition();
			ball->ResetPosition(Vec3(0, 0, _zdepth[4]));
		}
		else
		{
			_updater = &GameManager::Result;
			CCAudioMng::GetInstance().CkPlaySE("win");
		}
		return;
	}

	Sprite* sp = Sprite::create();
	Rect rect;
	for (int i = 0; i < _scores.size(); ++i)
	{
		sp = (Sprite*)UI->getChildByName("score" + std::to_string(i + 1));

		sp->setTextureRect(Rect(100 * (_scores[i] % 5), 100 * (_scores[i] / 5), 100, 100));
		sp->setVisible(true);
	}

	int pNum = (_isPlayer ? 1 : 2);
	std::string name = "score" + std::to_string(pNum);
	if (_invCnt % 60 == 0)
	{
		CCAudioMng::GetInstance().CkPlaySE("score");
	}

	if (_invCnt / (_mimSecond / 2) % 2)
	{
		UI->getChildByName(name)->setVisible(false);
	}
	--_invCnt;
}

void GameManager::Result()
{
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");
	/// UI画像を全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}

	UI->getChildByName("win")->setVisible(true);

	if (!CCAudioMng::GetInstance().IsPlaySE("win"))
	{
		UI->getChildByName("win")->setVisible(false);
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, TitleScene::createScene(), Color3B::WHITE));
	}
}

void GameManager::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		// トランジション中は処理を通さない
		return;
	}
	/// 関数ポインタの中身の処理を行う
	(this->*_updater)();
}
