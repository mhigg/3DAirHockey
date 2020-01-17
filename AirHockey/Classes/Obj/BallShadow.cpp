#include "BallShadow.h"
#include "Ball.h"
#include "../Manager/GameManager.h"

USING_NS_CC;

BallShadow::BallShadow(cocos2d::Vec3 vec)
{
	_localPos = vec;
	_shadowPlace = static_cast<SHADOW>(0);
	Init();
}

BallShadow::BallShadow()
{
	_shadowPlace = static_cast<SHADOW>(2);
	Init();
}

BallShadow::~BallShadow()
{
}

bool BallShadow::Init(void)
{
	// �摜
	setTexture("image/ball_shadow.png");
	auto size = getContentSize();

	// �������W
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	switch (_shadowPlace)
	{
	case UP:
		_localPos = { 0, visibleSize.height / 2 + size.height/2,0 };
		break;
	case DOWN:
		_localPos = { 0, -visibleSize.height / 2 - size.height/2,0 };
		break;
	case LEFT:
		// �摜�̉�]
		setRotation(90);
		_localPos = { -visibleSize.width / 2 - size.height / 2,0,0 };
		break;
	case RIGHT:
		// �摜�̉�]
		setRotation(90);
		_localPos = { visibleSize.width / 2 + size.height / 2,0,0 };
		break;
	default:
		_localPos = { 0,0,0 };
		break;
	};

	// pos�ƽ��ײĂ̑傫������_�����}�@�ɒu��������
	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

	return true;
}

void BallShadow::update(float dt)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		/// �Q�[���V�[���ȊO�̎��͏����ɓ���Ȃ��悤�ɂ���
		return;
	}
	/// �Q�[���}�l�[�W���[�̎擾
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");
	/// �{�[���̎擾
	auto ball = (Ball*)gameMng->getChildByName("ball");

	switch (_shadowPlace)
	{
	case UP:
	case DOWN:
		_localPos.x = ball->GetLocalPos().x;
		_localPos.z = ball->GetLocalPos().z;
		break;
	case LEFT:
	case RIGHT:
		_localPos.y = ball->GetLocalPos().y;
		_localPos.z = ball->GetLocalPos().z;
		break;
		break;
	default:
		_localPos = { 0,0,0 };
		break;
	};

	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));
}
