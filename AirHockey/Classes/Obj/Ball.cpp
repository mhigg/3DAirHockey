#include "Ball.h"
#include "StageWall.h"
#include "../Character/Player.h"
#include "../Manager/GameManager.h"
#include "../Manager/AnimMng.h"
#include "TrajectControl.h"
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

cocos2d::Vec3 Ball::GetLocalPos(void) const
{
	return _localPos;
}

std::tuple<bool, bool, bool> Ball::GetIsReverse() const
{
	return _isReverse;
}

bool Ball::Init(void)
{
	// �������W
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,0 };

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

	/// ����̏�����
	_isReverse = { false, false, false };

	/// �{�[���̋O������p�N���X�̐���
	_traject.reset(new TrajectControl());

	/// �x�W�F�Ȑ����g���Ē[�_�𐶐����Ă���(��)�@��
	_traject->CalBezierPoint();

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

	return true;
}

void Ball::ChangeIsReverse()
{
	/// �Q�[���}�l�[�W���[�̎擾
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// �v���C���[�̎擾
	auto player = (Player*)gameMng->getChildByName("player");

	/// ���]�t���O�̍X�V(X)
	if (_localPos.x - _radius < -gameMng->GetMovingRange().x)
	{
		std::get<0>(_isReverse) = false;
	}
	else if (_localPos.x + _radius > gameMng->GetMovingRange().x)
	{
		std::get<0>(_isReverse) = true;
	}
	else {}

	if (_localPos.y - _radius < -gameMng->GetMovingRange().y)
	{
		std::get<1>(_isReverse) = false;
	}
	else if (_localPos.y + _radius > gameMng->GetMovingRange().y)
	{
		std::get<1>(_isReverse) = true;
	}
	else {}

	if (_localPos.z > _wallDepth[29])
	{
		std::get<2>(_isReverse) = true;
	}
	else if (_localPos.z < player->GetDepth())
	{
		/// �v���C���[�ƃ{�[���̓����蔻��		��
		/// (������m�F���邽�߂Ɉꎟ�ϐ��ɕۑ����Ă���)		
		auto col = Collision::GetInstance().HitCollision2D(this->getPosition(), this->getContentSize(),
														   player->getPosition(), player->getContentSize());
		if (col)
		{
			std::get<2>(_isReverse) = false;
		}
	}
	else {}
}


void Ball::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// �Q�[���V�[���ȊO�̎��͏����ɓ���Ȃ��悤�ɂ���
		return;
	}

	/// �Ƃ肠�������ł���Ă݂Ă�
	if (!_isCurve)
	{
		_isCurve = _traject->CalBezierPoint();;
	}

	/// ���]�t���O�̕ύX���s���Ă���
	ChangeIsReverse();

	/// �{�[�����ړ������Ă���
	_localPos += _traject->GetVel(State::NORMAL);

	// �ǂ̐F�X�V
	auto director = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
	StageWall* wall;
	for (int k = 0; k < _wallDepth.size(); k++)
	{
		wall = (StageWall*)director->getChildByName("Wall" + std::to_string(k));

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