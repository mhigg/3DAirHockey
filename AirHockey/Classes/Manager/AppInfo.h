#pragma once
#include <memory>
#include "cocos2d.h"
#include "NetworkLogic.h"

// ��`
#define lpAppInfo AppInfo::GetInstance()

class AppInfo : public cocos2d::Ref
{
public:
	static AppInfo& GetInstance(void)
	{
		return (*s_Instance);
	}

	bool isHost(void);			// �z�X�g���ǂ�����Ԃ�
	void isHost(bool isHost);	// �z�X�g���ǂ�����o�^����
	const EG_CHAR* appID(void);			// �A�v���P�[�V����ID��Ԃ�
	void appID(const EG_CHAR* appID);	// �A�v���P�[�V����ID��o�^����

private:
	AppInfo();
	~AppInfo();

	// �V���O���g���̃f���[�^�[
	struct AppInfoDeleter
	{
		void operator()(AppInfo* memory)
		{
			delete memory;
		}
	};

	bool _isHost;			// �z�X�g�Ȃ�true, �Q�X�g�Ȃ�false
	const EG_CHAR* _appID;	// �A�v���P�[�V������ID

	static std::unique_ptr<AppInfo, AppInfoDeleter> s_Instance;
};

