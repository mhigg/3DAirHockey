#pragma once
#include "cocos2d.h"
#include <utility>

// using�錾
using Z_DEPTH		= std::pair<float, float>;	// ���s�̎n�_�ƏI�_
using VANISHIN_P	= std::pair<float, float>;	// �����_��X��Y�̍��W

class PointWithDepth
	:public cocos2d::Point, public cocos2d::Node
{
public:
	PointWithDepth();
	~PointWithDepth();

	void SetWorldPosition(float local_x, float local_y, float local_z);	// scale�ƍ��W��1�_�����}�@�Ōv�Z����
	float GetScale(void);												// ���ײėp��scare�̹ޯ��

	void SetVanishingPoint(cocos2d::Point pos);							// �����_�𓮂��������Ƃ��Ɏg������

private:
	void PointNormalize(cocos2d::Point VanishingPoint);					// 
	Z_DEPTH _zDepth;						// ���s�̎n�_�ƏI�_
	cocos2d::Point _firstVanishingPoint;	// �����̏����_��X��Y�̍��W
	cocos2d::Point _nowVanishingPoint;		// �X�V��������_��X��Y�̍��W
	cocos2d::Point _normalizePoint;			// 
	cocos2d::Point _magnification;			// �����_�𓮂����{��()

	float _scale;														// ���ײĂ̑傫��
};