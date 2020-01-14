#include "PointWithDepth.h"

std::unique_ptr<PointWithDepth, PointWithDepth::PointWithDepthDeleter> PointWithDepth::s_Instance(new PointWithDepth());	// �ݽ�ݽ�p�ư��߲���

#define Z_START_POINT	(0)			// Z(���s�̎n�_)
#define Z_END_POINT		(1200)		// Z(���s�̏I�_)
#define X_MAGNIFICATION (3.0f)		// X�̏����_�𓮂����{��
#define Y_MAGNIFICATION (3.0f)		// Y�̏����_�𓮂����{��
#define X_VanishingPoint_MAX (1024)	// X�����ɏ����_�𓮂����ő�l
#define Y_VanishingPoint_MAX (576)	// Y�����ɏ����_�𓮂����ő�l

PointWithDepth::PointWithDepth()
{
	// ���s�̐ݒ�
	_zDepth = std::make_pair(Z_START_POINT, Z_END_POINT);

	// �����_�̐ݒ�(�Ƃ肠�����ŉ�ʒ����ɐݒ�)
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_firstVanishingPoint = { 1024 / 2, 576 / 2 };
	_nowVanishingPoint = _firstVanishingPoint;

	// �����_�𓮂����{��
	_magnification = { X_MAGNIFICATION, Y_MAGNIFICATION };

	_moveVanishingPoint = { 0,0,X_VanishingPoint_MAX,Y_VanishingPoint_MAX };
	// �傫���̏�����
	_scale = 0;

}

PointWithDepth::~PointWithDepth()
{
}

cocos2d::Point PointWithDepth::SetWorldPosition(cocos2d::Vec3 localPos)
{
	// ���ײĂ̑傫���̐ݒ�
	GetScale(localPos.z);

	// X��Y�̍��W�v�Z
	PointNormalize(_nowVanishingPoint);

	cocos2d::Point pos;

	// ���s�̎�
	pos.x = _firstVanishingPoint.x + (_scale - 1) * (_nowVanishingPoint.x /** _magnification.x*/ * _normalizePoint.x);
	pos.y = _firstVanishingPoint.y + (_scale - 1) * (_nowVanishingPoint.y /** _magnification.y*/ * _normalizePoint.y);

	// ���W�̍X�V
	pos.x += (_scale) * (localPos.x);
	pos.y += (_scale) * (localPos.y);

	return pos;
}

float PointWithDepth::GetScale(float local_z)
{
	// ���ײĂ̑傫���̐ݒ�(1�`0)�܂�
	return _scale = (local_z - _zDepth.second) / (_zDepth.first - _zDepth.second);;
}

void PointWithDepth::SetVanishingPoint(cocos2d::Point pos)
{
	auto tmpVanishingPoint = _nowVanishingPoint + pos;

	if (tmpVanishingPoint.x < _moveVanishingPoint.origin.x + 1)
	{
		tmpVanishingPoint.x = _moveVanishingPoint.origin.x + 1;
	}
	else if (tmpVanishingPoint.x > _moveVanishingPoint.size.width)
	{
		tmpVanishingPoint.x = _moveVanishingPoint.size.width;
	}
	else if (tmpVanishingPoint.y < _moveVanishingPoint.origin.y + 1)
	{
		tmpVanishingPoint.y = _moveVanishingPoint.origin.y + 1;
	}
	else if (tmpVanishingPoint.y > _moveVanishingPoint.size.height)
	{
		tmpVanishingPoint.y = _moveVanishingPoint.size.height;
	}
	_nowVanishingPoint = tmpVanishingPoint;
}

void PointWithDepth::PointNormalize(cocos2d::Point vanishingPoint)
{
	// -1~1�܂ł̐��������
	_normalizePoint.x = ((_firstVanishingPoint.x / vanishingPoint.x) - 1);
	_normalizePoint.y = ((_firstVanishingPoint.y / vanishingPoint.y) - 1);
}
