#include "EffectMng.h"

std::unique_ptr<EffectMng, EffectMng::EffectMngDeleter> EffectMng::s_Instance(new EffectMng());	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

// ｴﾌｪｸﾄの登録(ｴﾌｪｸﾄのefk名,付けたい名前,大きさ,再生ｽﾋﾟｰﾄﾞ)
efk::EffectEmitter* EffectMng::EffectInit(const std::string& fileName,const std::string useName,float scale, float speed)
{
	// manager作成
	_manager.emplace(useName, efk::EffectManager::create(cocos2d::Director::getInstance()->getVisibleSize()));
	
	// ﾌｧｲﾙの読み込み
	auto effect = efk::Effect::create(fileName);
	// mapに追加
	_emitter.emplace(useName, efk::EffectEmitter::create(_manager[useName]));
	_emitter[useName]->setEffect(effect);
	// 大きさ
	_emitter[useName]->setScale(scale);
	// 再生ｽﾋﾟｰﾄﾞ
	_emitter[useName]->setSpeed(speed);
	// 再生が終わっても破棄しないように設定
	_emitter[useName]->setRemoveOnStop(false);
	return _emitter[useName];
}

ManagerMap EffectMng::GetManager(void)
{
	return _manager;
}

// ｴﾌｪｸﾄの再生(すでに再生中の場合は通らない)
bool EffectMng::PlayEffect(std::string useName)
{
	// 指定したｴﾌｪｸﾄを再生していなかったら
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->play();
	}

	return true;
}

// ｴﾌｪｸﾄの再生(すでに再生中の場合は通らない)
bool EffectMng::PlayEffect(std::string useName, cocos2d::Vec2 pos)
{
	// 指定したｴﾌｪｸﾄを再生していなかったら
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->setPosition(pos);
		_emitter[useName]->play();
	}

	return true;
}

// ｴﾌｪｸﾄの回転
bool EffectMng::SetRotateEffect(std::string useName, cocos2d::Vec3 rotate)
{
	// 現在と違ったら改めてｾｯﾄ
	if (_emitter[useName]->getRotation3D() != rotate)
	{
		_emitter[useName]->setRotation3D(rotate);
		
		return true;
	}
	return false;
}

// ｴﾌｪｸﾄの移動
bool EffectMng::SetPosEffect(std::string useName, cocos2d::Vec2 pos)
{
	// 現在と違ったら改めてｾｯﾄ
	if (_emitter[useName]->getPosition() != pos)
	{
		_emitter[useName]->setPosition(pos);
		return true;
	}
	return false;
}

// ｴﾌｪｸﾄの停止
bool EffectMng::StopEffect(std::string useName)
{
	// 指定したｴﾌｪｸﾄを再生してたら
	if (_emitter[useName]->isPlaying())
	{
		_emitter[useName]->stop();
	}
	return true;
}

// ｴﾌｪｸﾄが再生中かどうか(trueで再生中)
bool EffectMng::CheckEffect(std::string useName)
{
	if (_emitter[useName]->isPlaying())
	{
		return true;
	}
	return false;
}

EffectMng::EffectMng()
{
	
}

EffectMng::~EffectMng()
{

}
