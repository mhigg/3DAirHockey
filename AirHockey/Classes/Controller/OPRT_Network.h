#pragma once
#include "OPRT_State.h"
#include "NetworkLogic.h"

class OPRT_Network :
	public OPRT_State
{
public:
	OPRT_Network(cocos2d::Node* sp);
	OPRT_Network(cocos2d::Node* sp, bool isHost);
	~OPRT_Network();

	void Update(void);					// �ʐM�̐ڑ����ް��X�V����, ������ް��𑗐M����
	cocos2d::Vec2 GetPoint(void) const;	// ���肩�瑗�M���ꂽ���W�ް����擾

private:
	void Run(void);	// �ʐM�̐ڑ�
	void addParticle(int playerNr, float x, float y);

	cocos2d::Vec2 _point;
	bool _isHost;	// νĂȂ�true, �޽ĂȂ�false
	bool _swallowsTouches;
	NetworkLogic* _networkLogic;
};
