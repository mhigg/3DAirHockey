#pragma once

#include <memory>
#include <array>
#include "Controller/OPRT_State.h"
#include "../Obj/Obj.h"

/// プレイヤーのアンカーポイント
enum class PL_ANC
{
	CENTER,
	LEFTUP,
	RIGHTUP,
	LEFTDOWN,
	RIGHTDOWN,
	MAX
};

/// 画像名の差分を保存するための型

// first : ボールと当たっていない時, second : ボールと当たった時
using texturePair  = std::pair<cocos2d::Texture2D*, cocos2d::Texture2D*>;
using textureArray = std::array<texturePair, static_cast<int>(PL_ANC::MAX)>;


class Player : 
	public Obj
{
public:
	Player(const float& zdepth);
	~Player();

	/// 深度値の取得用
	float GetDepth() const;

	/// 設定したアンカーの座標取得用
	cocos2d::Vec2 GetAnchorPos(const std::string& name);

	/// 移動量取得用
	cocos2d::Vec2 GetMoveDistance() const;

	/// ボールと当たった時の画像変更用
	void ChangeImage(const int& num);
private:
	void InitTextureInfo();		// 差分画像の初期化
	void MoveUpdate();			// 移動更新用
	void VanishPointUpdate(const cocos2d::Vec2& pos);		// 消失点の更新用
	void ResetTexture();		// 最初に設定した画像に戻す
	void update(float dt);

	std::unique_ptr<OPRT_State> _oprtState;
	textureArray _texInfo;		// 画像情報保存用

	cocos2d::Vec2 _vel;			// 消失点の速度
	cocos2d::Vec2 _vPoint;		// 消失点の座標
	cocos2d::Vec2 _prePos;		// 移動前の座標

	int _dispCnt;				// 当たった時の画像を表示する時間
};

