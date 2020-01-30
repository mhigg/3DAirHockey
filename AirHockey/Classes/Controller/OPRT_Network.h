#pragma once
#include "OPRT_State.h"
#include "NetworkLogic.h"

class OPRT_Network :
	public OPRT_State
{
public:
	OPRT_Network(cocos2d::Node* sp);
	OPRT_Network(cocos2d::Node* sp, bool isHost);
	~OPRT_Network();

	void Update(void);					// 通信の接続とﾃﾞｰﾀ更新をし, 相手にﾃﾞｰﾀを送信する
	cocos2d::Vec2 GetPoint(void) const;	// 相手から送信された座標ﾃﾞｰﾀを取得

private:
	void Run(void);	// 通信の接続
	void addParticle(int playerNr, float x, float y);

	bool _isHost;	// ﾎｽﾄならtrue, ｹﾞｽﾄならfalse
	bool _swallowsTouches;
	NetworkLogic* _networkLogic;
};
