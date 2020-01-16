#pragma once
#include "cocos2d.h"

// 定義
#define lpAnimMng AnimMng::GetInstance()

class AnimMng : public cocos2d::Ref
{
public:
	static AnimMng& GetInstance()	// ｱﾄﾞﾚｽが欲しいためﾎﾟｲﾝﾀｰか参照で
	{
		return (*s_Instance);		// *で中身(ｸﾗｽの実体)を返す
	}

	// ｱﾆﾒｰｼｮﾝの登録(ﾌｧｲﾙの名前,付けたい名前,登録する枚数,ｱﾆﾒｰｼｮﾝ1枚当たりのﾌﾚｰﾑ数,ﾙｰﾌﾟするか)
	void AddAnimCache(const std::string spriteName, const std::string animName, const int endNum, const float delay, const bool returnAnim);
	void SetAnim(cocos2d::Sprite* sprite, const std::string animName, const bool repeat = false);	// ｱﾆﾒｰｼｮﾝのｾｯﾄ(trueで反転)

	void SetAnimReverseFlag(bool animReverseFlag);
private:
	// 関数ｵﾌﾞｼﾞｪｸﾄ
	// ﾃﾞｽﾄﾗｸﾀを呼ぶ
	struct AnimMngDeleter
	{
		void operator()(AnimMng* animMng)const
		{
			delete animMng;
		}
	};
	AnimMng();
	~AnimMng();

	static std::unique_ptr<AnimMng, AnimMngDeleter> s_Instance;	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

	bool _animReverseFlag;		// ｱﾆﾒｰｼｮﾝ用反転ﾌﾗｸﾞ
	bool _oldAnimReverseFlag;	// 1ﾌﾚｰﾑ前のｱﾆﾒｰｼｮﾝ用反転ﾌﾗｸﾞ
	std::string _animName;		// 現在のｱﾆﾒｰｼｮﾝの名前保存用
};