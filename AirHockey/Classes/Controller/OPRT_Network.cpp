#include "OPRT_Network.h"
#include "../ConsoleOut.h"
#include "../Manager/AppInfo.h"

USING_NS_CC;

OPRT_Network::OPRT_Network(cocos2d::Node* sp)
{
	// Photonネットワーククラスのインスタンスを作成
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), lpAppInfo.appID());

	/// マウス用のリスナーを生成している
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// マウスの情報を取得している
		auto mouse = ((EventMouse*)event);

		// マウスカーソルの座標を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getCursorX());
		eventContent->put<int, float>(2, mouse->getCursorY());
		_networkLogic->sendEvent(1, eventContent);
	};

	// イベントディスパッチャにシングルタップ用リスナーを追加する
	sp->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, sp);
}

OPRT_Network::OPRT_Network(cocos2d::Node* sp, bool isHost)
{
	_isHost = isHost;
	// Photonネットワーククラスのインスタンスを作成
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), lpAppInfo.appID());

	/// マウス用のリスナーを生成している
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// マウスの情報を取得している
		auto mouse = ((EventMouse*)event);

		// マウスカーソルの座標を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getCursorX());
		eventContent->put<int, float>(2, mouse->getCursorY());
		_networkLogic->sendEvent(1, eventContent);
	};

	// イベントディスパッチャにシングルタップ用リスナーを追加する
	sp->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, sp);
}


OPRT_Network::~OPRT_Network()
{
}

void OPRT_Network::Run(void)
{
	_networkLogic->run();
	switch (_networkLogic->getState())
	{
	case STATE_CONNECTED:
	case STATE_LEFT:
		if(_isHost)
		{
			// ゲスト側で、ルームが存在すればジョイン
			if (_networkLogic->isRoomExists())
			{
				_networkLogic->setLastInput(INPUT_2);
			}
		}
		else
		{
			// ホスト側ならルーム作成
			_networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// 接続が切れたら再度接続
		_networkLogic->connect();
		break;
	case STATE_CONNECTING:
	case STATE_JOINING:
	case STATE_JOINED:
	case STATE_LEAVING:
	case STATE_DISCONNECTING:
	default:
		break;
	}
}

void OPRT_Network::Update(void)
{
	Run();

	// データの送信：EventDispacherの命令に任せる→あえてここに書かなくていい

	// ここでﾃﾞｰﾀ受信
	// GetPointで渡す_pointの値の更新
	while (!_networkLogic->eventQueue.empty())
	{
		std::array<float, 3>arr = _networkLogic->eventQueue.front();
		_networkLogic->eventQueue.pop();

		int playerNr = static_cast<int>(arr[0]);
		_point.x = arr[1];
		_point.y = arr[2];
		CCLOG("%d, %f, %f", playerNr, _point.x, _point.y);
	}
}

cocos2d::Vec2 OPRT_Network::GetPoint(void) const
{
	return _point;
}

void OPRT_Network::addParticle(int playerNr, float x, float y)
{
	ParticleSystem* particle;
	switch (playerNr)
	{
	case 1:
		particle = ParticleFire::create();
		break;
	case 2:
		particle = ParticleSmoke::create();
		break;
	case 3:
		particle = ParticleFlower::create();
		break;
	default:
		particle = ParticleSun::create();
		break;
	}
	particle->setDuration(0.1);
	particle->setSpeed(500);
	particle->setPosition(cocos2d::Point(x, y));
	this->addChild(particle);
}