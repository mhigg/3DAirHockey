#pragma once
#include <array>
#include "Obj.h"

/// ボールの残像生成用クラス
class BallAfter :
	public Obj
{
public:
	BallAfter();
	~BallAfter();

	/// 座標更新用(ボールのローカル座標を取得する)
	void Update(const cocos2d::Vec3& lPos);
private:
	void Init();
	void update(float dt);

	std::array<cocos2d::Vec3, 30> _points;
	std::array<cocos2d::Sprite*, 30> _images;
};

