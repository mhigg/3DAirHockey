#include "OPRT_Network.h"
#include "../ConsoleOut.h"

// ���[���𕡐���邽�߂̃A�v���P�[�V�����h�c
// appID1, 2, 3�ō�郋�[���E���郋�[�����ς��܂�
static const EG_CHAR* appID1 = L"91ccb37c-1396-43af-bbbf-46a4124935a5";
static const EG_CHAR* appID2 = L"b1723cd8-6b7c-4d52-989c-702c2848d8e8";
static const EG_CHAR* appID3 = L"ac500b19-8cfa-47b5-9781-4d9d438496e4";

USING_NS_CC;

OPRT_Network::OPRT_Network(cocos2d::Node* sp)
{
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), appID2);

	_swallowsTouches = true;

	// �V���O���^�b�v���X�i�[��p�ӂ���
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(_swallowsTouches);

	// �e�C�x���g�̊��蓖��
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event/*CC_CALLBACK_2(OPRT_Network::onTouchBegan, this*/)
	{
		if (_networkLogic->playerNr)
		{
			this->addParticle(_networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);

			// �C�x���g�i�^�b�`���W�j�𑗐M
			ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
			eventContent->put<int, float>(1, touch->getLocation().x);
			eventContent->put<int, float>(2, touch->getLocation().y);
			_networkLogic->sendEvent(1, eventContent);
		}

		return true;
	};
	listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	listener->onTouchCancelled = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
	sp->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sp);
}

OPRT_Network::OPRT_Network(cocos2d::Node* sp, bool isHost)
{
	_isHost = isHost;
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), appID2);

	_swallowsTouches = true;

	// �V���O���^�b�v���X�i�[��p�ӂ���
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(_swallowsTouches);

	// �e�C�x���g�̊��蓖��
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (_networkLogic->playerNr)
		{
			this->addParticle(_networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);

			// �C�x���g�i�^�b�`���W�j�𑗐M
			ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
			eventContent->put<int, float>(1, touch->getLocation().x);
			eventContent->put<int, float>(2, touch->getLocation().y);
			_networkLogic->sendEvent(1, eventContent);
		}

		return true;
	};
	listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	listener->onTouchCancelled = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
	sp->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sp);
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
		// �Q�X�g���ŁA���[�������݂���΃W���C��
		if(!_isHost)
		{
			if (_networkLogic->isRoomExists())
			{
				_networkLogic->setLastInput(INPUT_2);
			}
		}
		else
		{
			_networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// �ڑ����؂ꂽ��ēx�ڑ�
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

	// �f�[�^�̑��M�FEventDispacher�̖��߂ɔC���遨�����Ă����ɏ����Ȃ��Ă���

	// �������ް��̍X�V
	// GetPoint�œn��_point�̒l�̍X�V
}

cocos2d::Vec2 OPRT_Network::GetPoint(void) const
{
	// �ް��̎�M�Ə���
	cocos2d::Vec2 retVec;
	if (_networkLogic->eventQueue.empty())
	{
		// event�����������Ƃ���-p999��Ԃ�
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