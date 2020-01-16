#include "AnimMng.h"

std::unique_ptr<AnimMng, AnimMng::AnimMngDeleter> AnimMng::s_Instance(new AnimMng());	// ²İ½Àİ½—pÕÆ°¸Îß²İÀ°

void AnimMng::AddAnimCache(const std::string spriteName, const std::string animName, const int endNum, const float delay, const bool returnAnim)
{
	// plist‚Ì“Ç‚İ‚İ
	auto cache = cocos2d::SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("image/" + spriteName + +"/" + spriteName + ".plist");

	cocos2d::Animation *animation = cocos2d::Animation::create();

	// ±ÆÒ°¼®İ‚ÌºÏ”•ªŒJ‚è•Ô‚·
	for (int k = 1; k <= endNum; k++)
	{
		// plist‚©‚ç•¡”‚Ì½Ìß×²Ä‚ğ’Ç‰Á
		cocos2d::SpriteFrame* sp = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName + "_" + std::to_string(k) + ".png");
		animation->addSpriteFrame(sp);
	}

	// ±ÆÒ°¼®İ‚ÌŠÔŠu
	animation->setDelayPerUnit(delay);

	// ÅŒã‚É‚È‚Á‚½‚çÅ‰‚É–ß‚é
	animation->setRestoreOriginalFrame(returnAnim);

	// ·¬¯¼­‚É“o˜^
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	animationCache->addAnimation(animation, animName);
}

void AnimMng::SetAnim(cocos2d::Sprite* sprite,const std::string animName, const bool repeat)
{
	if (_animName == animName 
		&& _oldAnimReverseFlag == _animReverseFlag)
	{
		return;
	}

	// ±ÆÒ°¼®İ‚ğ~‚ß‚é
	sprite->stopAllActions();

	// ·¬¯¼­‚©‚ç‚Á‚Ä‚­‚é
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	cocos2d::Animation *animation = animationCache->getAnimation(animName);
	auto action = cocos2d::Animate::create(animation);
	
	// ”½“]‚·‚é‚©‚Ç‚¤‚©(true‚Å”½“])
	auto flip = cocos2d::FlipX::create(_animReverseFlag);
	_oldAnimReverseFlag = _animReverseFlag;
	sprite->runAction(flip);

	// ±ÆÒ°¼®İ
	if (repeat)
	{
		_animName = animName;
		// Ù°Ìß—L
		sprite->runAction(cocos2d::RepeatForever::create(action));
	}
	else
	{
		_animName = animName;
		// –³‚µ
		sprite->runAction(action);
	}
}

void AnimMng::SetAnimReverseFlag(bool animReverseFlag)
{
	_animReverseFlag = animReverseFlag;
}

AnimMng::AnimMng()
{
	_animName = "";
	_animReverseFlag = false;
}

AnimMng::~AnimMng()
{
}

