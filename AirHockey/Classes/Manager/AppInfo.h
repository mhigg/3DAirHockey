#pragma once
#include <memory>
#include "cocos2d.h"
#include "NetworkLogic.h"
//#include "LoadBalancing-cpp/inc/Client.h"

// 定義
#define lpAppInfo AppInfo::GetInstance()

class AppInfo : public cocos2d::Ref
{
public:
	static AppInfo& GetInstance(void)
	{
		return (*s_Instance);
	}

	bool isHost(void);			// ホストかどうかを返す
	void isHost(bool isHost);	// ホストかどうかを登録する
	const EG_CHAR* appID(void);			// アプリケーションIDを返す
	void appID(const EG_CHAR* appID);	// アプリケーションIDを登録する
	void IncreasePeer(void);	// ルーム参加人数を加算
	void DecreasePeer(void);	// ルーム参加人数を減算
	int getJoiningPeer(void);	// 現在のルーム参加人数を取得

private:
	AppInfo();
	~AppInfo();

	// シングルトンのデリーター
	struct AppInfoDeleter
	{
		void operator()(AppInfo* memory)
		{
			delete memory;
		}
	};

	bool _isHost;			// ホストならtrue, ゲストならfalse
	const EG_CHAR* _appID;	// アプリケーションのID
	int _joiningPeers;		// ルーム参加人数

	static std::unique_ptr<AppInfo, AppInfoDeleter> s_Instance;
};

