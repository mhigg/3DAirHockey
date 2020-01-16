#include "Ball.h"
#include "StageWall.h"
#include "../Character/Player.h"
#include "../Manager/GameManager.h"
#include "../Manager/AnimMng.h"
#include "Collision.h"

USING_NS_CC;

Ball::Ball(std::vector<float> depth)
{
	_wallDepth = depth;
	Init();
}

Ball::Ball()
{
	Init();
}

Ball::~Ball()
{
}

cocos2d::Vec3 Ball::GetLocalPos(void)
{
	return _localPos;
}


void Ball::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// �Q�[���V�[���ȊO�̎��͏����ɓ���Ȃ��悤�ɂ���
		return;
	}
	/// �Q�[���}�l�[�W���[�̎擾
	auto gameMng  = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// �v���C���[�̎擾
	auto player = (Player*)gameMng->getChildByName("player");

	if (_localPos.z > _wallDepth[29])
	{
		zReverse = true;
	}
	else if (_localPos.z < player->GetDepth())
	{
		/// �v���C���[�ƃ{�[���̓����蔻��
		/// (������m�F���邽�߂Ɉꎟ�ϐ��ɕۑ����Ă���)
		auto col = Collision::GetInstance().HitCollision2D(this->getPosition(), this->getContentSize(),
														   player->getPosition(), player->getContentSize());
		if (col)
		{
			zReverse = false;
		}
	}
	else{}

	if (!zReverse)
	{
		_localPos.z+=3;
	}
	else
	{
		_localPos.z-=3;
	}
	
	if (_localPos.x - _radius < -gameMng->GetMovingRange().x)
	{
		xReverse = false;
	}
	else if (_localPos.x + _radius > gameMng->GetMovingRange().x)
	{
		xReverse = true;
	}
	else{}

	if (!xReverse)
	{
		_localPos.x += 2;
	}
	else
	{
		_localPos.x -= 2;
	}
	auto debug = gameMng->GetMovingRange();
	if (_localPos.y - _radius < -gameMng->GetMovingRange().y)
	{
		yReverse = false;
	}
	else if (_localPos.y + _radius > gameMng->GetMovingRange().y)
	{
		yReverse = true;
	}
	else{}

	if (!yReverse)
	{
		_localPos.y += 2;
	}
	else
	{
		_localPos.y -= 2;
	}

	// �ǂ̐F�X�V
	auto director = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
	for (int k = 0; k < _wallDepth.size(); k++)
	{
		auto wall = (StageWall*)director->getChildByName("Wall" + std::to_string(k));

		if (_localPos.z > _wallDepth[k])
		{
			// �ύX��̶װ
			wall->SetWallColorChangeFlag(true);
		}
		else
		{
			// �ʏ펞�̶װ
			wall->SetWallColorChangeFlag(false);
		}
	}

	// ���W�̍X�V
	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));


}

bool Ball::Init(void)
{
	// �������W
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,_wallDepth[0] };

	// �ްق̱�Ұ��ݓo�^
	lpAnimMng.AddAnimCache("ball", "ball", 15, 0.03f, false);

	// ������Ұ���
	lpAnimMng.SetAnim(this, "ball", false);

	// ���a(�摜�̑傫��/2 - �]��)
	_radius = 192 / 2 - 32;

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

	return true;
}
