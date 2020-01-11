#include "EffectMng.h"

std::unique_ptr<EffectMng, EffectMng::EffectMngDeleter> EffectMng::s_Instance(new EffectMng());	// ²Ý½ÀÝ½—pÕÆ°¸Îß²ÝÀ°

// ´Ìª¸Ä‚Ì“o˜^(´Ìª¸Ä‚Ìefk–¼,•t‚¯‚½‚¢–¼‘O,‘å‚«‚³,Ä¶½Ëß°ÄÞ)
efk::EffectEmitter* EffectMng::EffectInit(const std::string& fileName,const std::string useName,float scale, float speed)
{
	// managerì¬
	_manager.emplace(useName, efk::EffectManager::create(cocos2d::Director::getInstance()->getVisibleSize()));
	
	// Ì§²Ù‚Ì“Ç‚Ýž‚Ý
	auto effect = efk::Effect::create(fileName);
	// map‚É’Ç‰Á
	_emitter.emplace(useName, efk::EffectEmitter::create(_manager[useName]));
	_emitter[useName]->setEffect(effect);
	// ‘å‚«‚³
	_emitter[useName]->setScale(scale);
	// Ä¶½Ëß°ÄÞ
	_emitter[useName]->setSpeed(speed);
	// Ä¶‚ªI‚í‚Á‚Ä‚à”jŠü‚µ‚È‚¢‚æ‚¤‚ÉÝ’è
	_emitter[useName]->setRemoveOnStop(false);
	return _emitter[useName];
}

ManagerMap EffectMng::GetManager(void)
{
	return _manager;
}

// ´Ìª¸Ä‚ÌÄ¶(‚·‚Å‚ÉÄ¶’†‚Ìê‡‚Í’Ê‚ç‚È‚¢)
bool EffectMng::PlayEffect(std::string useName)
{
	// Žw’è‚µ‚½´Ìª¸Ä‚ðÄ¶‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->play();
	}

	return true;
}

// ´Ìª¸Ä‚ÌÄ¶(‚·‚Å‚ÉÄ¶’†‚Ìê‡‚Í’Ê‚ç‚È‚¢)
bool EffectMng::PlayEffect(std::string useName, cocos2d::Vec2 pos)
{
	// Žw’è‚µ‚½´Ìª¸Ä‚ðÄ¶‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç
	if (!_emitter[useName]->isPlaying())
	{
		_emitter[useName]->setPosition(pos);
		_emitter[useName]->play();
	}

	return true;
}

// ´Ìª¸Ä‚Ì‰ñ“]
bool EffectMng::SetRotateEffect(std::string useName, cocos2d::Vec3 rotate)
{
	// Œ»Ý‚Æˆá‚Á‚½‚ç‰ü‚ß‚Ä¾¯Ä
	if (_emitter[useName]->getRotation3D() != rotate)
	{
		_emitter[useName]->setRotation3D(rotate);
		
		return true;
	}
	return false;
}

// ´Ìª¸Ä‚ÌˆÚ“®
bool EffectMng::SetPosEffect(std::string useName, cocos2d::Vec2 pos)
{
	// Œ»Ý‚Æˆá‚Á‚½‚ç‰ü‚ß‚Ä¾¯Ä
	if (_emitter[useName]->getPosition() != pos)
	{
		_emitter[useName]->setPosition(pos);
		return true;
	}
	return false;
}

// ´Ìª¸Ä‚Ì’âŽ~
bool EffectMng::StopEffect(std::string useName)
{
	// Žw’è‚µ‚½´Ìª¸Ä‚ðÄ¶‚µ‚Ä‚½‚ç
	if (_emitter[useName]->isPlaying())
	{
		_emitter[useName]->stop();
	}
	return true;
}

// ´Ìª¸Ä‚ªÄ¶’†‚©‚Ç‚¤‚©(true‚ÅÄ¶’†)
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
