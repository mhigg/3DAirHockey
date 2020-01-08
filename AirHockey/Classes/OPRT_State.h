#pragma once
#include "cocos2d.h"

// 入力端末の選択
enum class INPUT_DEVICE
{
	KEY,
	TOUCH,
	MAX
};

// ｷｰ入力用enum
enum class INPUT_ID
{
	LEFT,	// 左
	RIGHT,	// 右
	UP,		// 上
	DOWN,	// 下
	BTN_1,	// ﾎﾞﾀﾝ1
	BTN_2,	// ﾎﾞﾀﾝ2
	BTN_ESC,// ESCﾎﾞﾀﾝ
	ALL_KEY,// 全てのｷｰの状態取得用
	MAX
};

// ｷｰの入力状態
enum class TRIGGER_STATE
{
	ON_NON,		// 押した瞬間(1ﾌﾚｰﾑ)
	ON,			// 押しつづけているとき
	OFF_NON,	// 離した瞬間(1ﾌﾚｰﾑ)
	OFF,		// 離しつづけているとき
	MAX
};

// using宣言
using KeyMap = std::map<INPUT_ID, TRIGGER_STATE>;	// ｷｰと入力状態を入れるMAP

// for分用ｵﾍﾟﾚｰﾀｰのｵｰﾊﾞｰﾛｰﾄﾞ
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

	TRIGGER_STATE GetState(INPUT_ID inputId);	// 指定されたｷｰの取得
private:
	
protected:
	KeyMap state;	// ｷｰと入力状態を入れるMAP

};