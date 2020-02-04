/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#pragma once
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "Controller/OPRT_State.h"

class TitleScene 
	: public cocos2d::Scene
{
public:
	~TitleScene();
	static cocos2d::Scene* createScene();

	virtual bool init();							// 初期化用関数
	void update(float dt);							// 状態遷移用

	void menuCloseCallback(cocos2d::Ref* pSender);	// ｳｨﾝﾄﾞｳを閉じる為のﾎﾞﾀﾝ
	void GyroButton(cocos2d::Ref* ref);

	// implement the "static create()" method manually
	CREATE_FUNC(TitleScene);


	std::unique_ptr<OPRT_State> _oprtState;	// ﾀｯﾁやｼﾞｬｲﾛの入力取得用
	bool _isGyro;
	cocos2d::Label* _androidLabel;
};

#endif // __TitleScene_H__
