#pragma once

#include <memory>
#include <array>
#include "Controller/OPRT_State.h"

/// �v���C���[�̃A���J�[�|�C���g
enum class PL_ANC
{
	CENTER,
	LEFTUP,
	RIGHTUP,
	LEFTDOWN,
	RIGHTDOWN,
	MAX
};

/// �摜���̍�����ۑ����邽�߂̌^

// first : �{�[���Ɠ������Ă��Ȃ���, second : �{�[���Ɠ���������
using texturePair  = std::pair<cocos2d::Texture2D*, cocos2d::Texture2D*>;
using textureArray = std::array<texturePair, static_cast<int>(PL_ANC::MAX)>;


class Player : 
	public cocos2d::Sprite
{
public:
	Player(const float& zdepth);
	~Player();

	/// �[�x�l�̎擾�p
	float GetDepth() const;

	/// �ݒ肵���A���J�[�̍��W�擾�p
	cocos2d::Vec2 GetAnchorPos(const std::string& name);

	/// �ړ��ʎ擾�p
	cocos2d::Vec2 GetMoveDistance() const;

	/// �{�[���Ɠ����������̉摜�ύX�p
	void ChangeImage(const int& num);
private:
	void InitTextureInfo();		// �����摜�̏�����
	void MoveUpdate();			// �ړ��X�V�p
	void ResetTexture();		// �ŏ��ɐݒ肵���摜�ɖ߂�
	void update(float dt);

	std::unique_ptr<OPRT_State> _oprtState;
	textureArray _texInfo;		// �摜���ۑ��p

	cocos2d::Vec2 _prePos;		// �ړ��O�̍��W
	cocos2d::Vec3 _localPos;	

	int _dispCnt;				// �����������̉摜��\�����鎞��
};

