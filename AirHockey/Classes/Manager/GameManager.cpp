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
	_moveRange(1024 / 2, 576 / 2), _numSize(100)
{
	Init();
	/// ゲームマネージャーの名前設定
	this->setName("GameManager");

	/// 関数ポインターの初期化
	_updater = &GameManager::Connect;

	/// インターバルの初期化
	_invCnt = _mimSecond * 4 ;
	
	/// プレイヤーの得点の初期化
	for (auto& score : _scores)
	{
		score = 0;
	}
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

void GameManager::GeneratePlayer(bool isHost, bool setFlag)
{
	/// プレイヤーの生成
	int	  layer;
	float depth;
	Player* player;
	for (int i = 0; i < _scores.size(); ++i)
	{
		/// プレイヤーの深度値を設定している( ? 手前：奥)
		depth = (i == 0 ? _zdepth[1] : _zdepth[_wallMax - 2]);
		layer = (i == 0 ? static_cast<int>(SpriteNum::PLAYER) : static_cast<int>(SpriteNum::SHADOW));
		player = new Player(isHost, depth, i);

		player->GyroSet(setFlag);
		player->setName("player" + std::to_string(i + 1));

		/// プレイヤーの追加
		this->addChild(player, layer);

		isHost ^= 1;	// 一人目と二人目でホストとゲストを分けるため
	}
}

bool GameManager::IsGame() const
{
	/// ゲーム中かの判定を返している
	return (_updater == &GameManager::Game);
}

void GameManager::TransitionScore(bool isPlayer)
{
	_isPlayer	= isPlayer;
	
	/// プレイヤーの得点加算
	if (_isPlayer)
	{
		/// 1P
		++_scores[0];
	}
	else
	{
		/// 2P
		++_scores[1];
	}
	/// ゲーム中の残像を消している
	this->getChildByName("ballAfter")->setVisible(false);

	/// インターバルの設定
	_invCnt		= _mimSecond * 4 + 1;

	/// 得点表示に移行する
	_updater	= &GameManager::Score;
}

void GameManager::Init()
{
	/// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float mag = _maxDepth / (_wallMax * _wallMax);

	float depth;

	/// 深度値の設定
	for (int x = _wallMax; x > 0; x--)
	{
		depth = x * x * mag;
		depth = _maxDepth - depth;
		_zdepth.emplace_back(depth);
	}
	/// 深度値の始点の初期化
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

	/// 残像の生成
	auto ballAfter = new BallAfter();
	ballAfter->setName("ballAfter");
	ballAfter->setVisible(false);
	this->addChild(ballAfter, static_cast<int>(SpriteNum::BALL));
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
	/// UI情報の取得
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");

	/// UIを全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}
	
	Sprite* sp = Sprite::create();
	if (_invCnt < _mimSecond)
	{
		/// ゲーム開始の合図音が鳴り終わった時に入る
		if (!CCAudioMng::GetInstance().IsPlaySE("start"))
		{
			/// ゲームモードに移行する
			_updater = &GameManager::Game;

			/// ボールの位置をリセットさせる
			auto ball = (Ball*)this->getChildByName("ball");
			ball->ResetPosition({0,0, _zdepth[_zdepth.size() / 2 - 1]});
			this->getChildByName("ballAfter")->setVisible(true);
			return;
		}

		/// ゲームスタート合図のUI表示
		sp = (Sprite*)UI->getChildByName("start");
		sp->setVisible(true);
	}
	else
	{
		/// カウントダウンのUI表示
		sp = (Sprite*)UI->getChildByName("cntDown");
		sp->setVisible(true);

		/// 表示する
		Rect rect = Rect(_numSize * (_invCnt / _mimSecond), 0, 
						 _numSize, _numSize);
		sp->setTextureRect(rect);

		/// カウントダウンの音を再生する
		if (_invCnt % _mimSecond == 0 && _invCnt > _mimSecond)
		{
			CCAudioMng::GetInstance().CkPlaySE("cntDown");
		}
	}

	/// スタートを知らせる音を再生する
	if (_invCnt == _mimSecond)
	{
		CCAudioMng::GetInstance().CkPlaySE("start");
	}

	/// インターバルの減算
	--_invCnt;
}

void GameManager::Game()
{
	/// ボールクラスが動作を行っている
}

void GameManager::Score()
{
	/// UI情報の取得
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");

	/// UIを全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}

	if (_invCnt <= 0)
	{
		bool isGame = true;			/// ゲームを継続させるかの判定用
		for (auto score : _scores)
		{
			/// ゲームを継続するかの判定を取得している
			if (score >= 5)
			{
				isGame = false;
				break;
			}
		}

		if (isGame)
		{
			/// ゲーム継続
			_updater = &GameManager::Stay;
			_invCnt  = _mimSecond * 4;

			/// ボールの位置をリセットさせる
			Ball* ball = (Ball*)this->getChildByName("ball");
			BallAfter* ballAfter = (BallAfter*)this->getChildByName("ballAfter");
			ballAfter->ResetPosition();
			ball->ResetPosition(Vec3(0, 0, _zdepth[4]));
		}
		else
		{
			/// ゲーム終了
			_updater = &GameManager::Result;
			CCAudioMng::GetInstance().CkPlaySE("win");
		}
		return;
	}

	/// 得点のUI表示
	Sprite* sp = Sprite::create();
	Rect rect;
	for (int i = 0; i < _scores.size(); ++i)
	{
		sp = (Sprite*)UI->getChildByName("score" + std::to_string(i + 1));

		sp->setTextureRect(Rect(_numSize * (_scores[i] % 5), _numSize * (_scores[i] / 5), 
								_numSize, _numSize));
		sp->setVisible(true);
	}

	/// 得点を入れたプレイヤーのUIを点滅させている
	int pNum = (_isPlayer ? 1 : 2);
	std::string name = "score" + std::to_string(pNum);

	/// スコアの点滅中、表示されたとき音を再生させる
	if (_invCnt % _mimSecond == 0)
	{
		CCAudioMng::GetInstance().CkPlaySE("score");
	}

	/// UIの点滅表示
	if ((_invCnt / (_mimSecond / 2)) % 2)
	{
		UI->getChildByName(name)->setVisible(false);
	}

	/// インターバルの減算
	--_invCnt;
}

void GameManager::Result()
{
	/// UI情報の取得
	auto UI = Director::getInstance()->getRunningScene()->getChildByName("UI");

	/// UI画像を全て非表示にしている
	for (auto sp : UI->getChildren())
	{
		sp->setVisible(false);
	}

	/// 勝利時のUI表示
	UI->getChildByName("win")->setVisible(true);

	/// 勝利時に流れるシングル音が停止した時、タイトルへ移行する
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
