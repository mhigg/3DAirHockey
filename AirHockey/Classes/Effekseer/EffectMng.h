#pragma once
#include "cocos2d.h"
#include "Effekseer.h"

// 定義
#define lpEffectMng EffectMng::GetInstance()

// ｴﾌｪｸﾄの情報保存用Map
using ManagerMap = std::map<std::string, efk::EffectManager*>;
using EmitterMap = std::map<std::string, efk::EffectEmitter*>;

class EffectMng : public cocos2d::Ref
{
public:
	static EffectMng& GetInstance()	// ｱﾄﾞﾚｽが欲しいためﾎﾟｲﾝﾀｰか参照で
	{
		return (*s_Instance);		// *で中身(ｸﾗｽの実体)を返す
	};

	ManagerMap GetManager(void);						// Sceneのvisitやupdateで使う_managerのｹﾞｯﾀｰ

	// ｴﾌｪｸﾄの登録(ｴﾌｪｸﾄのefk名,付けたい名前,大きさ,再生ｽﾋﾟｰﾄﾞ)
	efk::EffectEmitter* EffectInit(const std::string& fileName, const std::string useName, float scale, float speed);
	bool PlayEffect(std::string useName);								// ｴﾌｪｸﾄの再生(すでに再生中の場合は通らない)
	bool PlayEffect(std::string useName , cocos2d::Vec2 pos);			// ｴﾌｪｸﾄの再生(すでに再生中の場合は通らない)
	bool SetRotateEffect(std::string useName, cocos2d::Vec3 rotate);	// ｴﾌｪｸﾄの回転
	bool SetPosEffect(std::string useName, cocos2d::Vec2 pos);			// ｴﾌｪｸﾄの移動
	bool StopEffect(std::string useName);								// ｴﾌｪｸﾄの停止
	bool CheckEffect(std::string useName);								// ｴｴｸﾄが再生中かどうか(再生中ならtrue)

private:
	// 関数ｵﾌﾞｼﾞｪｸﾄ
	// ﾃﾞｽﾄﾗｸﾀを呼ぶ
	struct EffectMngDeleter
	{
		void operator()(EffectMng* effectMng)const
		{
			delete effectMng;
		}
	};
	EffectMng();
	~EffectMng();

	static std::unique_ptr<EffectMng, EffectMngDeleter> s_Instance;		// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

	ManagerMap _manager;						// EffectManagerｲﾝｽﾀﾝｽ用ﾎﾟｲﾝﾀｰ
	EmitterMap _emitter;						// ｴﾌｪｸﾄ保存用Map
};
