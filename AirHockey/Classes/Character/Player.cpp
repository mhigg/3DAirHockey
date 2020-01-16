#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/Input_Touch.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

USING_NS_CC;

float Player::_depth;

Player::Player()
{
	/// ���̉摜��ǉ����Ă���@��
	auto sprite = Sprite::create("player.png");
	sprite->setName("image");
	this->setContentSize(sprite->getContentSize());
	/// �v���C���[�̃^�O����ݒ肵�Ă���(��)�@��
	this->setName("player");
	this->addChild(sprite);
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
	_depth = depth;

	return Player::create();
}

float Player::GetDepth() const
{
	return _depth;
}

void Player::MoveUpdate()
{
	/// ��ʃT�C�Y�̎擾
	auto scrSize = Director::getInstance()->sharedDirector()->getOpenGLView()->getFrameSize();
	auto pos	 = _oprtState->GetPoint();
	auto size	 = this->getChildByName("image")->getContentSize();

	/// X���̈ړ��͈̓`�F�b�N
	if ((pos.x + size.width / 2 < scrSize.width) &&
		(pos.x - size.width / 2 > 0))
	{
		this->setPositionX(pos.x);
	}

	/// Y���̈ړ��͈̓`�F�b�N
	if ((pos.y + size.height / 2 < scrSize.height) &&
		(pos.y - size.height / 2 > 0))
	{
		this->setPositionY(pos.y);
	}
}

void Player::update(float dt)
{
	MoveUpdate();
	
	// ���s���̐[���ɂ���āA�v���C���[�̃T�C�Y��ύX����悤�ɂ��Ă���
	setScale(lpPointWithDepth.GetInstance().GetScale(_depth));
	
	lpPointWithDepth.GetInstance().SetVanishingPoint(_oprtState->GetPoint() / 100 - cocos2d::Vec2(400,300) / 100);
}
