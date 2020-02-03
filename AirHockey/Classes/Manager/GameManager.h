#pragma once

#include <memory>
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
	void GeneratePlayer(bool isHost);

	// スコア表示の状態に遷移する
	void TransitionScore();

private:
	CREATE_FUNC(GameManager);

	void update(float dt);
	void Init();

	/// 関数ポインタを使って管理するもの
	void Stay();					// 待ち状態
	void Game();					// ゲーム中	
	void Score();					// スコアの表示などを行う

	std::vector<float> _zdepth;			// 深度値を保存するもの

	void (GameManager::*_updater)();	// ゲーム中の状態管理用

	bool _isHost;

	int _invCnt;	

	const cocos2d::Vec2 _moveRange;
	const float _maxDepth;	// 奥行の最大値
	const int _wallMax;		// 壁の最大数
	const int _playerDepth;
	
};

