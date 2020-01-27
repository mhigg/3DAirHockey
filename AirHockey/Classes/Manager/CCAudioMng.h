#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <jni.h>

/// cricket audio関係のヘッダーファイル
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"


/// Android Studioでの初期化(仮コメント)

#if CK_PLATFORM_ANDROID
#pragma execution_character_set("utf-8")

#ifndef __ANDROID_JNI__
#define __ANDROID_JNI__

#ifdef __cplusplus
extern "C"
{
#endif
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initCricket(JNIEnv *env, jclass obj, jobject context);
#ifdef __cplusplus
}
#endif
#endif

#endif

// bankデータ保持用のunordered_map
using CkBankMap  = std::unordered_map<std::string, CkBank*>;
// bankデータ保持用のunordered_map
using CkSoundMap = std::unordered_map<std::string, CkSound*>;

class CCAudioMng
{
public:
	static CCAudioMng& GetInstance()
	{
		if (s_Instance == nullptr)
		{
			/// メモリが生成されず、スタックオーバーフローが起こる
			s_Instance.reset(new CCAudioMng);
		}
		return *s_Instance;
	}

	// cricket audioを再生するためのもの
	void Update();

	// cricket audioの機能を終了させる
	void ShutDown();

	// bankデータの登録を行っている(パス読み込みの形式はファイルパスの指定を初期設定にしている)
	void RegistBank(const std::string& pathName, const std::string& key);
	
	// bankデータから効果音の登録を行っている
	void RegistBankSE(const std::string& bankKey, const std::string& bankName, const std::string& seKey);

	// cksファイルからBGMの登録を行っている
	void RegistStreamBGM(const std::string& pathName, const std::string& key);

	// 効果音の再生
	void CkPlaySE(const std::string& key);
	void CkPlaySE(const std::string& key, const float& rate);

	// BGMの再生
	void CkPlayBGM(const std::string& key);
	void CkPlayBGM(const std::string& key, const float& rate);

	void CkStopBGM(const std::string& key);
private:
	CCAudioMng();
	~CCAudioMng();

	void CkStopSE(const std::string& key);

	// 一定範囲の割合を返すもの(minRate : 音量の最小値, maxRate : 音の最大値)
	float Clamp(const float& rate, 
				const float& minRate = 0.2f, 
				const float& maxRate = 1.0f);

	// 登録しているサウンドデータの解放用
	void Destroy();

	// シングルトンのデリーター
	struct AudioDeleter
	{
		void operator()(CCAudioMng* memory)
		{
			delete memory;
		}
	};
	static std::unique_ptr<CCAudioMng, AudioDeleter> s_Instance;

	/// unordered_mapを使用して管理を行う
	CkBankMap _bankMap;		// bankデータの管理用
	CkSoundMap _seMap;		// 効果音データの管理用
	CkSoundMap _bgmMap;		// BGMデータの管理用
};

