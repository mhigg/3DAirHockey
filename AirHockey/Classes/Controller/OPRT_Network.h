#pragma once
#include "OPRT_State.h"
#include "NetworkLogic.h"

class OPRT_Network :
	public OPRT_State
{
public:
	OPRT_Network();
	OPRT_Network(bool isHost);
	~OPRT_Network();

	void Update(void);					// �ʐM�̐ڑ����ް��X�V����, ������ް��𑗐M����
	cocos2d::Vec2 GetPoint(void) const;	// ���肩�瑗�M���ꂽ���W�ް����擾

		// �e�X�g�p
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

private:
	void Run(void);

	bool _isHost;	// νĂȂ�true, �޽ĂȂ�false

	void addParticle(int playerNr, float x, float y);
	bool _swallowsTouches;

	NetworkLogic* networkLogic;
};
