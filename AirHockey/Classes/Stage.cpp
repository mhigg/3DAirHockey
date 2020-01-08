#include "Stage.h"

Stage::Stage()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// ã‰º¶‰E‡
	std::array<cocos2d::Vec2, 2> _square;
	_square = { cocos2d::Vec2{-300,-250},cocos2d::Vec2{300,250} };
	float nomalSize = abs(_square[0].x) + abs(_square[1].x);

	auto a = cocos2d::DrawNode::create();
	a->drawTriangle(cocos2d::Vec2{ visibleSize.width/2 - 300,0}, cocos2d::Vec2{ visibleSize.width/2,0 }, cocos2d::Vec2{ visibleSize.width / 2 ,visibleSize.height / 2 }, cocos2d::Color4F::GREEN);
	this->addChild(a);

	auto b = cocos2d::DrawNode::create();
	b->drawTriangle(cocos2d::Vec2{ visibleSize.width / 2,0 }, cocos2d::Vec2{ visibleSize.width / 2 + 300,0 }, cocos2d::Vec2{ visibleSize.width / 2 ,visibleSize.height / 2 }, cocos2d::Color4F::BLUE);
	b->setAnchorPoint(cocos2d::Vec2{ 0.5, 0 });
	this->addChild(b);


	//for (float z = 0; z <= 2000; z += 200)
	//{
	//	auto a = cocos2d::DrawNode::create();
	//	float w = nomalSize + ((2000 - nomalSize) * (z / nomalSize));
	//	float scale = nomalSize / w;
	//	cocos2d::Vec2 startPos = { _square[0].x * scale,_square[0].y * scale };
	//	cocos2d::Vec2 endPos = { _square[1].x * scale ,_square[1].y * scale };
	//	a->drawRect(startPos, endPos, cocos2d::Color4F::GREEN);
	//	a->setPosition(visibleSize / 2);
	//	this->addChild(a);
	//};


	a->runAction(cocos2d::SkewTo::create(1, 40, 0));
	b->runAction(cocos2d::SkewTo::create(1, 40, 0));

	this->scheduleUpdate();
}

Stage::~Stage()
{
}

void Stage::update(float dt)
{
	
}

