#include "AnimMng.h"

std::unique_ptr<AnimMng, AnimMng::AnimMngDeleter> AnimMng::s_Instance(new AnimMng());	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

void AnimMng::AddAnimCache(const std::string spriteName, const std::string animName, const int endNum, const float delay, const bool returnAnim)
{
	// plistの読み込み
	auto cache = cocos2d::SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("image/" + spriteName + +"/" + spriteName + ".plist");

	cocos2d::Animation *animation = cocos2d::Animation::create();

	// ｱﾆﾒｰｼｮﾝのｺﾏ数分繰り返す
	for (int k = 1; k <= endNum; k++)
	{
		// plistから複数のｽﾌﾟﾗｲﾄを追加
		cocos2d::SpriteFrame* sp = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName + "_" + std::to_string(k) + ".png");
		animation->addSpriteFrame(sp);
	}

	// ｱﾆﾒｰｼｮﾝの間隔
	animation->setDelayPerUnit(delay);

	// 最後になったら最初に戻る
	animation->setRestoreOriginalFrame(returnAnim);

	// ｷｬｯｼｭに登録
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	animationCache->addAnimation(animation, animName);
}

void AnimMng::SetAnim(cocos2d::Sprite* sprite,const std::string animName, const bool repeat)
{
	//if (_animName == animName 
	//	&& _oldAnimReverseFlag == _animReverseFlag)
	//{
	//	return;
	//}

	// ｱﾆﾒｰｼｮﾝを止める
	sprite->stopAllActions();

	// ｷｬｯｼｭから持ってくる
	cocos2d::AnimationCache *animationCache = cocos2d::AnimationCache::getInstance();
	cocos2d::Animation *animation = animationCache->getAnimation(animName);
	auto action = cocos2d::Animate::create(animation);
	
	// 反転するかどうか(trueで反転)
	auto flip = cocos2d::FlipX::create(_animReverseFlag);
	_oldAnimReverseFlag = _animReverseFlag;
	sprite->runAction(flip);

	// ｱﾆﾒｰｼｮﾝ
	if (repeat)
	{
		_animName = animName;
		// ﾙｰﾌﾟ有
		sprite->runAction(cocos2d::RepeatForever::create(action));
	}
	else
	{
		_animName = animName;
		// 無し
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

