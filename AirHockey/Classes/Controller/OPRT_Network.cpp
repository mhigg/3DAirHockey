#include "OPRT_Network.h"
#include "../ConsoleOut.h"

static const EG_CHAR* appID1 = L"91ccb37c-1396-43af-bbbf-46a4124935a5";
static const EG_CHAR* appID2 = L"b1723cd8-6b7c-4d52-989c-702c2848d8e8";

USING_NS_CC;

OPRT_Network::OPRT_Network()
{
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	networkLogic = new NetworkLogic(&ConsoleOut::get(), appID1);

	_swallowsTouches = true;

	// �V���O���^�b�v���X�i�[��p�ӂ���
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(_swallowsTouches);

	// �e�C�x���g�̊��蓖��
	listener->onTouchBegan = CC_CALLBACK_2(OPRT_Network::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OPRT_Network::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OPRT_Network::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(OPRT_Network::onTouchCancelled, this);

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

OPRT_Network::OPRT_Network(bool isHost)
{
	_isHost = isHost;
	// Photon�l�b�g���[�N�N���X�̃C���X�^���X���쐬
	networkLogic = new NetworkLogic(&ConsoleOut::get(), appID1);

	_swallowsTouches = true;

	// �V���O���^�b�v���X�i�[��p�ӂ���
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(_swallowsTouches);

	// �e�C�x���g�̊��蓖��
	listener->onTouchBegan = CC_CALLBACK_2(OPRT_Network::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OPRT_Network::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OPRT_Network::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(OPRT_Network::onTouchCancelled, this);

	// �C�x���g�f�B�X�p�b�`���ɃV���O���^�b�v�p���X�i�[��ǉ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}


OPRT_Network::~OPRT_Network()
{
}

void OPRT_Network::Run(void)
{
	networkLogic->run();
	switch (networkLogic->getState())
	{
	case STATE_CONNECTED:
	case STATE_LEFT:
		// �Q�X�g���ŁA���[�������݂���΃W���C��
		if(!_isHost)
		{
			if (networkLogic->isRoomExists())
			{
				networkLogic->setLastInput(INPUT_2);
			}
		}
		else
		{
			networkLogic->setLastInput(INPUT_1);
		}
		break;
	case STATE_DISCONNECTED:
		// �ڑ����؂ꂽ��ēx�ڑ�
		networkLogic->connect();
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

	// �������ް��̑��M
}

cocos2d::Vec2 OPRT_Network::GetPoint(void) const
{
	// �ް��̎�M�Ə���
	cocos2d::Vec2 retVec;
	while (!networkLogic->eventQueue.empty()) {
		std::array<float, 3>arr = networkLogic->eventQueue.front();
		networkLogic->eventQueue.pop();

		int playerNr = static_cast<int>(arr[0]);
		retVec.x = arr[1];
		retVec.y = arr[2];
		CCLOG("%d, %f, %f", playerNr, retVec.x, retVec.y);
	}

	return retVec;
}

bool OPRT_Network::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

	if (networkLogic->playerNr) {
		this->addParticle(networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);

		// �C�x���g�i�^�b�`���W�j�𑗐M
		ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
		eventContent->put<int, float>(1, touch->getLocation().x);
		eventContent->put<int, float>(2, touch->getLocation().y);
		networkLogic->sendEvent(1, eventContent);
	}

	return true;
}

void OPRT_Network::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void OPRT_Network::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void OPRT_Network::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void OPRT_Network::addParticle(int playerNr, float x, float y)
{
	ParticleSystem* particle;
	switch (playerNr) {
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