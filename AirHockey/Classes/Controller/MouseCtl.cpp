#include "MouseCtl.h"

USING_NS_CC;

MouseCtl::MouseCtl(cocos2d::Node* node)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	_point = cocos2d::Vec2((origin.x + visibleSize.width) / 2, origin.y + visibleSize.height / 2);

	/// マウス用のリスナーを生成している
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// マウスの情報を取得している
		auto mouse = ((EventMouse*)event);
		/// 移動対象に対して、マウスの座標を渡している
		_point = (Vec2(mouse->getCursorX(), mouse->getCursorY()));
	};

	/// マウス操作の登録を行っている(nodeの登録が必要ない場合、消す)
	auto eventDisPatcher = Director::getInstance()->getEventDispatcher();
	eventDisPatcher->addEventListenerWithSceneGraphPriority(mouseListener, node);
}

MouseCtl::~MouseCtl()
{
}

void MouseCtl::Update()
{
}

cocos2d::Vec2 MouseCtl::GetPoint() const
{
	return _point;
}
