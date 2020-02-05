#include "AppInfo.h"

std::unique_ptr<AppInfo, AppInfo::AppInfoDeleter> AppInfo::s_Instance(new AppInfo());	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

bool AppInfo::isHost(void)
{
	return _isHost;
}

void AppInfo::isHost(bool isHost)
{
	_isHost = isHost;
}

const EG_CHAR * AppInfo::appID(void)
{
	return _appID;
}

void AppInfo::appID(const EG_CHAR * appID)
{
	_appID = appID;
}

void AppInfo::IncreasePeer(void)
{
	_joiningPeers++;
}

void AppInfo::DecreasePeer(void)
{
	_joiningPeers--;
}

int AppInfo::getJoiningPeer(void)
{
	return _joiningPeers;
}

AppInfo::AppInfo()
{
	_isHost = true;
	_appID = L"ac500b19-8cfa-47b5-9781-4d9d438496e4";
	_joiningPeers = 0;
}


AppInfo::~AppInfo()
{
}
