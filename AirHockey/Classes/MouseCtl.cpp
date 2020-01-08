#include "MouseCtl.h"

USING_NS_CC;

MouseCtl::MouseCtl(cocos2d::Node* node)
{
	/// マウス用のリスナーを生成している
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// マウスの情報を取得している
		auto mouse = ((EventMouse*)event);
		/// 移動対象に対して、マウスの座標を渡している
		_mPos = (Vec2(mouse->getCursorX(), mouse->getCursorY()));
	};

	/// マウス操作の登録を行っている(nodeの登録が必要ない場合、消す)
	auto eventDisPatcher = Director::getInstance()->getEventDispatcher();
	eventDisPatcher->addEventListenerWithSceneGraphPriority(mouseListener, node);
}

MouseCtl::~MouseCtl()
{
}

cocos2d::Vec2 MouseCtl::GetPos() const
{
	return _mPos;
}
