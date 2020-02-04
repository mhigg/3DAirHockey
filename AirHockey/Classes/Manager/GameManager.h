#pragma once

#include <memory>
#include <array>
#include "cocos2d.h"

class Player;
class Ball;

// ZOrder用
enum class SpriteNum
{
	SHADOW,
	BALL,
	PLAYER,
};

class GameManager :
	public cocos2d::Node
{
public:
	GameManager();
	~GameManager();

	static GameManager* createGameMng();

	cocos2d::Vec2 GetMovingRange() const;
	std::vector<float> GetDepths() const;
	float GetMaxDepth(void) const;

	// ホストで入ってきたらtrue, ゲストで入ってきたらfalseを渡してプレイヤーを生成
	void GeneratePlayer(bool isHost, bool setFlag);

	// ゲーム中かの判定取得用
	bool IsGame() const;
	// スコア表示の状態に遷移する
	void TransitionScore(bool isPlayer);

private:
	CREATE_FUNC(GameManager);

	void update(float dt);
	void Init();

	/// 関数ポインタを使って管理するもの
	void Connect();					// 接続待機
	void Stay();					// 待ち状態
	void Game();					// ゲーム中	
	void Score();					// スコアの表示などを行う
	void Result();

	std::vector<float> _zdepth;			// 深度値を保存するもの

	void (GameManager::*_updater)();	// ゲーム中の状態管理用

	bool _isHost;
	bool _isPlayer;			// プレイヤー確認用 true : player1, false : player2

	int _invCnt;
	std::array<int, 2> _scores;		// プレイヤーのスコア管理用

	const int _mimSecond;	// 60秒(固定値)
	const cocos2d::Vec2 _moveRange;
	const float _maxDepth;	// 奥行の最大値
	const int _wallMax;		// 壁の最大数
	const int _playerDepth;
	
};

