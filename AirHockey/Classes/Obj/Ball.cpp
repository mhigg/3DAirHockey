#include "Ball.h"
#include "../Controller/OPRT_Key.h"
#include "../Character/Player.h"
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

void Ball::update(float dt)
{
	/// ���ݑ����Ă���V�[���̎擾
	auto runScene = Director::getInstance()->getRunningScene();
	/// �Q�[���V�[���������Ă��鎞�A�������s���悤�ɂ��Ă���
	if (runScene->getName() != "GameScene")
	{
		return;
	}
	/// �Q�[���}�l�[�W���[�̎擾
	auto gameMng  = runScene->getChildByName("gameLayer")->getChildByName("gameManager");
	/// �v���C���[�̎擾
	auto player = (Player*)gameMng->getChildByName("player");
	if (_localPos.z > _wallDepth[29])
	{
		zReverse = true;
	}
	else if (_localPos.z < player->GetDepth())
	{
		/// �v���C���[�̓����蔻��()
		/// �����蔻��͎�ꂽ
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
	
	if (_localPos.x - _radius < -350)
	{
		xReverse = false;
	}
	else if (_localPos.x + _radius > +350)
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
	
	if (_localPos.y - _radius < -250)
	{
		yReverse = false;
	}
	else if (_localPos.y + _radius > +250)
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

	// ���W�̍X�V
	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));


	if (_state->GetState(INPUT_ID::UP) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ 0, +3 });
	}
	if (_state->GetState(INPUT_ID::DOWN) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ 0, -3 });
	}
	if (_state->GetState(INPUT_ID::LEFT) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ -5, 0 });
	}
	if (_state->GetState(INPUT_ID::RIGHT) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ +5, 0 });
	}
//	lpEffectMng.PlayEffect("Ball", lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
}

bool Ball::Init(void)
{
	// �������W
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,_wallDepth[0] };

	// ø����
	setTexture("ball.png");
	// ���a(�摜�̑傫��/2)
	_radius = getContentSize().width / 2;

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	_state = new OPRT_Key(this);

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

	return true;
}
