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
		/// 移動対象に対して、マウスの座標を渡している
		_point = (Vec2(mouse->getCursorX(), mouse->getCursorY()));

		// イベント（タッチ座標）を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getLocation().x);
		eventContent->put<int, float>(2, mouse->getLocation().y);
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
		/// 移動対象に対して、マウスの座標を渡している
		_point = (Vec2(mouse->getCursorX(), mouse->getCursorY()));

		// イベント（タッチ座標）を送信
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getLocation().x);
		eventContent->put<int, float>(2, mouse->getLocation().y);
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

	// ここでﾃﾞｰﾀの更新
	// GetPointで渡す_pointの値の更新
}

cocos2d::Vec2 OPRT_Network::GetPoint(void) const
{
	// ﾃﾞｰﾀの受信と処理
	cocos2d::Vec2 retVec;
	if (_networkLogic->eventQueue.empty())
	{
		// eventが何も無いときは-p999を返す
		retVec = { -999,-999 };
	}
	else
	{
		while (!_networkLogic->eventQueue.empty())
		{
			std::array<float, 3>arr = _networkLogic->eventQueue.front();
			_networkLogic->eventQueue.pop();

			int playerNr = static_cast<int>(arr[0]);
			retVec.x = arr[1];
			retVec.y = arr[2];
			CCLOG("%d, %f, %f", playerNr, retVec.x, retVec.y);
		}
	}

	return retVec;
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