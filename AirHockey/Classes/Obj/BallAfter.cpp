#include "BallAfter.h"

USING_NS_CC;

BallAfter::BallAfter() : _invTime(10)
{
	Init();
}

BallAfter::BallAfter(const cocos2d::Vec3 & lPos) : _invTime(10)
{
	_localPos = lPos;
	Init();
}

BallAfter::~BallAfter()
{
}

void BallAfter::Update(const cocos2d::Vec3 & lPos)
{
	if ((_cnt / _invTime) % 2)
	{
		/// 3次元座標の更新
		for (int i = _images.size() - 2; i >= 0; --i)
		{
			_points[i + 1] = _points[i];
		}
		_cnt = 0;
		_points[0] = lPos;

		/// 更新した座標を使って、残像の位置とサイズを更新する
		for (int i = 0; i < _images.size(); ++i)
		{
			///// 位置調整のために、画面サイズの半分を減算している(直値なので、修正を必ず行う) ◆
			_images[i]->setPosition(lpPointWithDepth.SetWorldPosition(_points[i]) - cocos2d::Vec2(1024 / 2, 576 / 2));
			_images[i]->setScale(lpPointWithDepth.GetScale(_points[i].z));
		}
	}
	// 時間の更新
	++_cnt;
}

void BallAfter::Init()
{
	// 計算した画像の倍率を保存するもの
	float rate;
	for (int i = 0; i < _images.size(); ++i)
	{
		/// 画像サイズの倍率を計算している
		rate	= (float)(_images.size() - i) / (_images.size());

		/// 画像の取得
		_images[i]	= Sprite::create("image/ball/new_ball/ball_13.png");

		/// 画像サイズの設定
		_images[i]->setContentSize(_images[i]->getContentSize());

		/// 画像透明度の設定
		_images[i]->setOpacity(140 * rate);
		
		/// 初期座標の設定
		setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

		/// 画像スケールの設定
		setScale(lpPointWithDepth.GetScale(_localPos.z));

		this->addChild(_images[i]);
	}
}

void BallAfter::update(float dt)
{
}
