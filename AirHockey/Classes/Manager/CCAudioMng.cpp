#include <cmath>
#include "CCAudioMng.h"

std::unique_ptr<CCAudioMng, CCAudioMng::AudioDeleter> CCAudioMng::s_Instance;

/// criket audioの初期化(Android時)
#if CK_PLATFORM_ANDROID
#ifdef __cplusplus
extern "C"
{
#endif
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_initCricket(JNIEnv *env, jclass obj, jobject context)
	{
		CkConfig config(env, context);
		CkInit(&config);
	}
#ifdef __cplusplus
	}
#endif
#endif

CCAudioMng::CCAudioMng()
{
/// criket audioの初期化(windows時)
#if CK_PLATFORM_WIN	
	CkConfig config;
	CkInit(&config);   
#endif	
}

CCAudioMng::~CCAudioMng()
{
}

float CCAudioMng::Clamp(const float & rate, const float & minRate, const float & maxRate)
{
	return fmin(maxRate, fmax(minRate, rate));
}
void CCAudioMng::Destroy()
{
	/// bankデータの解放
	if (_bankMap.size() > 0)
	{
		for (auto& bank : _bankMap)
		{
			_bankMap[bank.first]->destroy();
		}
		_bankMap.clear();
	}

	/// 効果音データの解放
	if (_seMap.size() > 0)
	{
		for (auto& se : _seMap)
		{
			_seMap[se.first]->destroy();
		}
		_seMap.clear();
	}

	/// BGMデータの解放
	if (_bgmMap.size() > 0)
	{
		for (auto& bgm : _bgmMap)
		{
			_bgmMap[bgm.first]->destroy();
		}
		_bgmMap.clear();
	}
}

void CCAudioMng::ShutDown()
{
	Destroy();
	CkShutdown();
}

void CCAudioMng::RegistBank(const std::string & pathName, const std::string& key)
{
	CkPathType pathType;
/// 動作環境によって、読み込むパスのタイプを切り替えている
#if CK_PLATFORM_WIN
	pathType = CkPathType::kCkPathType_FileSystem;
#else
	pathType = CkPathType::kCkPathType_Default;
#endif

	/// バンクの登録を行っている
	if (_bankMap.find(key) == _bankMap.end())
	{
		/// Soundファイルの先頭パスを指定している
		std::string bankName = "Sound/" + pathName;
		_bankMap[key] = CkBank::newBank(bankName.c_str(), pathType);
	}
}

void CCAudioMng::RegistBankSE(const std::string & bankKey, const std::string & bankName, const std::string & seKey)
{
	/// bankのデータがマップに登録されているかの判定
	if (_bankMap.find(bankKey) != _bankMap.end())
	{
		/// 指定したキーの効果音が登録されているかの判定
		if (_seMap.find(seKey) == _seMap.end())
		{
			/// 効果音の登録を行う
			_seMap[seKey] = CkSound::newBankSound(_bankMap[bankKey], bankName.c_str());
		}
	}
}

void CCAudioMng::RegistStreamBGM(const std::string & pathName, const std::string & key)
{
	CkPathType pathType;
	/// 動作環境によって、読み込むパスのタイプを切り替えている
#if CK_PLATFORM_WIN
	pathType = CkPathType::kCkPathType_FileSystem;
#else
	pathType = CkPathType::kCkPathType_Default;
#endif

	if (_bgmMap.find(key) == _bgmMap.end())
	{
		/// Soundファイルの先頭パスを指定している
		std::string bgmName = "Sound/" + pathName;
		_bgmMap[key] = CkSound::newStreamSound(bgmName.c_str(), pathType);
		/// 音の大きさを半分にしてみた
		//_bgmMap[key]->setVolume(0.5f);
	}
}

void CCAudioMng::CkPlaySE(const std::string& key)
{
	/// 効果音が登録されているかの判定
	if (_seMap.find(key) != _seMap.end())
	{
		if (_seMap[key] != nullptr)
		{
			/// 再生中でなければ、再生を行う
			if (_seMap[key]->isPlaying())
			{
				CkStopSE(key);
				_seMap[key]->play();
			}
			else
			{
				_seMap[key]->play();
			}
		}
	}
}

void CCAudioMng::CkPlaySE(const std::string & key, const float & rate)
{
	/// 効果音が登録されているかの判定
	if (_seMap.find(key) != _seMap.end())
	{
		/// 再生中でなければ、再生を行う
		if (_seMap[key]->isPlaying())
		{
			CkStopSE(key);
			_seMap[key]->setVolume(Clamp(rate));
			_seMap[key]->play();
		}
		else
		{
			_seMap[key]->setVolume(Clamp(rate));
			_seMap[key]->play();
		}
	}
}

void CCAudioMng::CkPlayBGM(const std::string& key)
{
	/// BGMが登録されているかの判定
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// 再生中
		if (!_bgmMap[key]->isPlaying())
		{
			_bgmMap[key]->play();
		}
	}
}

void CCAudioMng::CkPlayBGM(const std::string & key, const float & rate)
{
	/// BGMが登録されているかの判定
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// 再生中
		if (_bgmMap[key]->isPlaying())
		{
			/// 音量の調整をして再生する
			_bgmMap[key]->setVolume(Clamp(rate));
			_bgmMap[key]->play();
		}
	}
}

void CCAudioMng::CkStopBGM(const std::string & key)
{
	/// BGMが登録されているかの判定
	if (_bgmMap.find(key) != _bgmMap.end())
	{
		/// 再生中
		if (_bgmMap[key]->isPlaying())
		{
			_bgmMap[key]->stop();
		}
	}
}

void CCAudioMng::CkStopSE(const std::string & key)
{
	/// BGMが登録されているかの判定
	if (_seMap.find(key) != _seMap.end())
	{
		/// 再生中
		if (_seMap[key]->isPlaying())
		{
			_seMap[key]->stop();
		}
	}
}

void CCAudioMng::Update()
{
	CkUpdate();
}
