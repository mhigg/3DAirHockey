#include "Input_Touch.h"

Input_Touch::Input_Touch()
{
}

Input_Touch::Input_Touch(cocos2d::Node * node)
{
	_point = cocos2d::Vec2(0, 0);
	// 初期化
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	// 押した瞬間
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{	
		_point = touch->getLocation();
		return true;
	};
	// 動かしている間
	listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		_point = touch->getLocation();
	};
	// 離した瞬間
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		_point = touch->getLocation();
	};
	node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
}

Input_Touch::~Input_Touch()
{
}

cocos2d::Vec2 Input_Touch::GetPoint(void) const
{
	return _point;
}

void Input_Touch::Update(void)
{
}