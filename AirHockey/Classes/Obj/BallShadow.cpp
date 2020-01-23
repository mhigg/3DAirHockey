#include "BallShadow.h"
#include "Ball.h"
#include "../Manager/GameManager.h"

USING_NS_CC;

BallShadow::BallShadow(int num, const std::string fileName)
{
	// �㉺���E�̉e�̂ǂꂩ��enum�ŕۑ�
	_shadowPlace = static_cast<SHADOW>(num);
	Init(fileName);
}

BallShadow::BallShadow()
{
	setTexture("image/ball_shadow.png");
	Init();
}

BallShadow::~BallShadow()
{
}

bool BallShadow::Init(void)
{
	//�摜
	//setTexture("image/ball_shadow.png");
	// ���ײĂ̑傫��
	auto size = getContentSize();

	// �������W
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	switch (_shadowPlace)
	{
	case UP:
		_localPos = { 0, visibleSize.height / 2 + size.height / 2,0 };
		break;
	case DOWN:
		_localPos = { 0, -visibleSize.height / 2 - size.height,0 };
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
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1�ڰт��Ƃ�update��
	cocos2d::Node::scheduleUpdate();

	return true;
}

bool BallShadow::Init(const std::string fileName)
{
	// �摜
	setTexture(fileName);
	Init();
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
	auto obj = (Obj*)gameMng->getChildByName("ball");

	// �摜�̊g��k���p
	float scale = 0;
	// ��ʻ���
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// ���W�Ƒ傫���̍X�V
	switch (_shadowPlace)
	{
	case UP:
		_localPos.x = obj->GetLocalPos().x;
		_localPos.z = obj->GetLocalPos().z;
		scale = (obj->GetLocalPos().y - visibleSize.height / 2) / (visibleSize.height) + 1;
		break;
	case DOWN:
		_localPos.x = obj->GetLocalPos().x;
		_localPos.z = obj->GetLocalPos().z;
		scale = (obj->GetLocalPos().y - visibleSize.height / 2) / (visibleSize.height);
		break;
	case LEFT:
		_localPos.y = obj->GetLocalPos().y;
		_localPos.z = obj->GetLocalPos().z;
		scale = (obj->GetLocalPos().x - visibleSize.width / 2) / (visibleSize.width);
		break;
	case RIGHT:
		_localPos.y = obj->GetLocalPos().y;
		_localPos.z = obj->GetLocalPos().z;
		scale = (obj->GetLocalPos().x - visibleSize.width / 2) / (visibleSize.width) + 1;
		break;
	default:
		_localPos = { 0,0,0 };
		break;
	};

	// �����x�̍X�V
	setOpacity(255 * abs(scale));

	// ��_�����}�@�ɂ������̍��W�̾��
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// ��_�����}�@�ɂ������̉摜�̻��ސݒ�
	setScale(lpPointWithDepth.GetScale(_localPos.z) * abs(scale));
}
