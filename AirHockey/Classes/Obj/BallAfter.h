#pragma once
#include <array>
#include "SpriteAffectDepth.h"

/// ボールの残像生成用クラス
class BallAfter :
	public SpriteAffectDepth
{
public:
	BallAfter();
	BallAfter(const cocos2d::Vec3& lPos);
	~BallAfter();

	/// 座標更新用(ボールのローカル座標を取得する)
	void Update(const cocos2d::Vec3& lPos);
	void ResetPosition();
private:
	void Init();
	void update(float dt);

	int _cnt = 0;				// 時間計測用

	std::array<cocos2d::Vec3, 6> _points;
	std::array<cocos2d::Sprite*, 6> _images;

	const int _invTime;			// 残像を移動させる間隔
};

