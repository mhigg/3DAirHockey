#pragma once
#include "cocos2d.h"

// ���͒[���̑I��
enum class INPUT_DEVICE
{
	KEY,
	TOUCH,
	MAX
};

// �����͗penum
enum class INPUT_ID
{
	LEFT,	// ��
	RIGHT,	// �E
	UP,		// ��
	DOWN,	// ��
	BTN_1,	// ����1
	BTN_2,	// ����2
	BTN_ESC,// ESC����
	ALL_KEY,// �S�Ă̷��̏�Ԏ擾�p
	MAX
};

// ���̓��͏��
enum class TRIGGER_STATE
{
	ON_NON,		// �������u��(1�ڰ�)
	ON,			// �����Â��Ă���Ƃ�
	OFF_NON,	// �������u��(1�ڰ�)
	OFF,		// �����Â��Ă���Ƃ�
	MAX
};

// using�錾
using KeyMap = std::map<INPUT_ID, TRIGGER_STATE>;	// ���Ɠ��͏�Ԃ�����MAP

// for���p���ڰ���̵��ް۰��
INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator++(INPUT_ID& key);
INPUT_ID operator* (INPUT_ID& key);

class OPRT_State :
	public cocos2d::Node
{
public:
	OPRT_State();
	~OPRT_State();

	virtual void Update(void) = 0;

	TRIGGER_STATE GetState(INPUT_ID inputId);	// �w�肳�ꂽ���̎擾
private:
	
protected:
	KeyMap state;	// ���Ɠ��͏�Ԃ�����MAP

};