#include "StageWall.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	// �ð��ø����(��)�̻���
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// �摜
	setTextureRect(deformationRect);
	// �ݶ��߲�Ă̐ݒ�
	setAnchorPoint({ 0.5,0.5 });
	// �F�̐ݒ�
	_normalColor = color;
	setColor(_normalColor);
	// ���݂̐F
	_nowColor = getColor();
	// �ύX�����ۂ̐F
	_changeColor = cocos2d::Color3B(
		0,
		191 - (191 * (zDepth / 1000)),
		255 - (255 * (zDepth / 1000)) );
	// �F�̕ύX�׸�
	_colorChangeFlag = false;

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

void StageWall::SetWallColorChangeFlag(bool flag)
{
	_colorChangeFlag = flag;
}

void StageWall::update(float dt)
{
	// ���W�̍X�V
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));

	// �F�̕ύX
	if (_colorChangeFlag && _nowColor != _changeColor)
	{
		// �ްْʉߎ��̐F�ɕύX
		WallColor(_changeColor);
	}
	else if (!_colorChangeFlag && _nowColor != _normalColor)
	{
		// �ʏ펞�̐F
		WallColor(_normalColor);
	}
	else{}
}

void StageWall::WallColor(cocos2d::Color3B color)
{
	// ���݂̐F�̍X�V
	_nowColor = color;
	// �F�ύX
	setColor(_nowColor);
}
