#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/Input_Touch.h"

USING_NS_CC;

Player::Player()
{
	/// ���̉摜��ǉ����Ă���@��
	auto sprite = Sprite::create("player.png");
	setTexture("player.png");

	this->addChild(sprite);

	/// �v���C���[�̃^�O����ݒ肵�Ă���(��)�@��
	this->setName("player");

	this->setPosition(Vec2::ZERO);
	/// ���̃}�E�X�ݒ�
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_oprtState.reset(new MouseCtl(this));
#else
	_oprtState.reset(new Input_Touch(this));
#endif

	this->scheduleUpdate();
}

Player::~Player()
{
}

Player * Player::createPlayer(const float& depth)
{
	//_depth = depth;
	return Player::create();
}

float Player::getDepth() const
{
	return debug;
}

void Player::update(float dt)
{
	/// ���W�̍X�V���s���Ă���
	this->setPosition(_oprtState->GetPoint());
}
