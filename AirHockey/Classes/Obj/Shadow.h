#pragma once
#include "Obj.h"
#include <vector>

// 影の位置
enum SHADOW
{
	UP,		// 上
	DOWN,	// 下
	LEFT,	// 左
	RIGHT,	// 右
	MAX
};


class Shadow :
	public Obj
{
public:
	Shadow(int num, const std::string fileName, std::string ID = "");
	Shadow();
	~Shadow();
private:
	bool Init(void);
	bool Init(const std::string fileName);
	void update(float dt)override;	// 状態更新

	SHADOW _shadowPlace;			// 上下左右の影のenum保存用
	std::string _typeName;
	std::string _id;
};
