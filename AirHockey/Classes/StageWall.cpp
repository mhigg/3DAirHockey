#include "StageWall.h"
#include "OPRT_Key.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Color3B* color)
{
	// �ð��ø����(��)�̻���
	//cocos2d::Rect deformationRect = { 0,0,size.x,size.y };

	// �摜
	setTexture("wall.png");
	// �ݶ��߲�Ă̐ݒ�
	setAnchorPoint({ 0.5,0.5 });
	// �F
	setColor(*color);
	// ���W
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	_pointDepth = new PointWithDepth();
	_pointDepth->SetWorldPosition(_localPos.x, _localPos.y, _localPos.z);
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(*_pointDepth);
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(_pointDepth->GetScale());

	_state = new OPRT_Key(this);

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();
}

StageWall::StageWall()
{
}

StageWall::~StageWall()
{
}

void StageWall::update(float dt)
{
	_pointDepth->SetWorldPosition(_localPos.x, _localPos.y, _localPos.z);
	setPosition(*_pointDepth);
	setScale(_pointDepth->GetScale());

	if (_state->GetState(INPUT_ID::UP) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ 0, +1 });
	}
	if (_state->GetState(INPUT_ID::DOWN) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ 0, -1 });
	}
	if (_state->GetState(INPUT_ID::LEFT) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ -1, 0 });
	}
	if (_state->GetState(INPUT_ID::RIGHT) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ +1, 0 });
	}
}
