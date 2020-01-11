#include "StageWall.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B * color)
{
	// �ð��ø����(��)�̻���
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// �摜
	setTextureRect(deformationRect);
	// �ݶ��߲�Ă̐ݒ�
	setAnchorPoint({ 0.5,0.5 });
	// �F
	setColor(*color);
	// ���W
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

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
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

}
