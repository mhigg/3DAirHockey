#include "MouseCtl.h"

USING_NS_CC;

MouseCtl::MouseCtl(cocos2d::Node* node)
{
	/// �}�E�X�p�̃��X�i�[�𐶐����Ă���
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// �}�E�X�̏����擾���Ă���
		auto mouse = ((EventMouse*)event);
		/// �ړ��Ώۂɑ΂��āA�}�E�X�̍��W��n���Ă���
		_mPos = (Vec2(mouse->getCursorX(), mouse->getCursorY()));
	};

	/// �}�E�X����̓o�^���s���Ă���(node�̓o�^���K�v�Ȃ��ꍇ�A����)
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
