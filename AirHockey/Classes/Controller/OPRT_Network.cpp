#include "OPRT_Network.h"
#include "../ConsoleOut.h"
#include "../Manager/AppInfo.h"

USING_NS_CC;

OPRT_Network::OPRT_Network(cocos2d::Node* sp)
{
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), lpAppInfo.appID());

	/// �}�E�X�p�̃��X�i�[�𐶐����Ă���
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// �}�E�X�̏����擾���Ă���
		auto mouse = ((EventMouse*)event);
		/// �ړ��Ώۂɑ΂��āA�}�E�X�̍��W��n���Ă���
		_point = (Vec2(mouse->getCursorX(), mouse->getCursorY()));

		// �C�x���g�i�^�b�`���W�j�𑗐M
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getLocation().x);
		eventContent->put<int, float>(2, mouse->getLocation().y);
		_networkLogic->sendEvent(1, eventContent);
	};

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
	sp->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, sp);
}

OPRT_Network::OPRT_Network(cocos2d::Node* sp, bool isHost)
{
	_isHost = isHost;
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	_networkLogic = new NetworkLogic(&ConsoleOut::get(), lpAppInfo.appID());

	/// �}�E�X�p�̃��X�i�[�𐶐����Ă���
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = [&](Event* event)
	{
		/// �}�E�X�̏����擾���Ă���
		auto mouse = ((EventMouse*)event);
		/// �ړ��Ώۂɑ΂��āA�}�E�X�̍��W��n���Ă���
		_point = (Vec2(mouse->getCursorX(), mouse->getCursorY()));

		// �C�x���g�i�^�b�`���W�j�𑗐M
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, mouse->getLocation().x);
		eventContent->put<int, float>(2, mouse->getLocation().y);
		_networkLogic->sendEvent(1, eventContent);
	};

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
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
			// �Q�X�g���ŁA���[�������݂���΃W���C��
			if (_networkLogic->isRoomExists())
			{
				_networkLogic->setLastInput(INPUT_2);
			}
		}
		else
		{
			// �z�X�g���Ȃ烋�[���쐬
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