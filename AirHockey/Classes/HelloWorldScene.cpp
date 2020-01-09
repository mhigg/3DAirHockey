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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Math.h"
#include "StageWall.h"
#include "Ball.h"
#include "player.h"

USING_NS_CC;
#define STAGE_SIZE_X (700)
#define STAGE_SIZE_Y (500)

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
	
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0,"Hello");
    }

	// ﾌｨｰﾙﾄﾞ用ﾚｲﾔｰ
	auto stageLayer = Layer::create();
	stageLayer->setName("StageLayer");
	this->addChild(stageLayer);

	// ｽﾃｰｼﾞの壁作成
	// 奥行の最大値
	float maxDepth = 1000;
	// 壁の最大数
	int wallMaxNum = 30;
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// 奥行の作成
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	// ﾌｨｰﾙﾄﾞの基本サイズ
	Point wallSize = { 700,500 };
	// ﾌｨｰﾙﾄﾞ用ｽﾌﾟﾗｲﾄの作成
	for (int k = 0; k < zdepth.size(); k++)
	{
		auto color = new Color3B(
			255 - (255 * (zdepth[k] / 1000)),
			255 - (255 * (zdepth[k] / 1000)),
			255 - (255 * (zdepth[k] / 1000)));
		auto stageWall = new StageWall({ 0, 0 }, zdepth[k], wallSize, color);
		// ｽﾃｰｼﾞﾚｲﾔｰに追加
		stageLayer->addChild(stageWall);
	}

	// ﾎﾞｰﾙ用ﾚｲﾔｰ
	auto ballLayer = Layer::create();
	ballLayer->setName("BallLayer");
	this->addChild(ballLayer);
	// ﾎﾞｰﾙ作成
	auto ball = new Ball(zdepth);
	ballLayer->addChild(ball);


	
	/// プレイヤーの生成
	auto charLayer = Layer::create();
	auto player = Player::createPlayer();
	charLayer->addChild(player);
	this->addChild(charLayer, static_cast<int>(LayerNum::PLAYER));

	this->scheduleUpdate();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::update(float dt)
{

	// auto visibleSize = Director::getInstance()->getVisibleSize();
	//// 上下左右順
	//std::array<Vec2, 2> _square;
	//_square = { Vec2{-300,-250},Vec2{300,250} };
	//float nomalSize = abs(_square[0].x) + abs(_square[1].x);


	//for (float z = 0; z <= 2000; z += 200)
	//{
	//	auto a = DrawNode::create();
	//	float w = nomalSize + ((2000 - nomalSize) * (z / nomalSize));
	//	float scale = nomalSize / w;
	//	Vec2 startPos = { _square[0].x * scale,_square[0].y * scale };
	//	Vec2 endPos = { _square[1].x * scale ,_square[1].y * scale };
	//	a->drawRect(startPos, endPos, Color4F::GREEN);
	//	a->setPosition(visibleSize / 2);
	//	this->addChild(a);
	//};

}
