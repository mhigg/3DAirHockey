﻿#include "NetworkLogic.h"
#include "Manager/AppInfo.h"

#ifdef _EG_EMSCRIPTEN_PLATFORM
#	include <emscripten.h>
#endif

//static const EG_CHAR* appID = L"91ccb37c-1396-43af-bbbf-46a4124935a5"; // set your app id here
static const EG_CHAR* appVersion = L"1.0";

static const bool autoLobbyStats = true;
static const int sendInterval = 1000;
static const bool useBestRegion = true; // attention: in RegionSelectionMode::BEST different client instances (sometimes even from the same location) might get different ping results and hence might connect to different regions - in that case they don't see each other or each others rooms!
#if defined EG_PLATFORM_SUPPORTS_CPP11 && defined EG_PLATFORM_SUPPORTS_MULTITHREADING
	// pinging takes a moment, so for a real game it makes sense to retrive the best region with getRegionWithBestPing(), store it in a file, use RegionSelectionMode::SELECT and pass that region to selectRegion() and to only use RegionSelectionMode::BEST, if no best region has been determined yet or if the player explicitly requests to repeat pinging
static const nByte regionSelectionMode = useBestRegion ? ExitGames::LoadBalancing::RegionSelectionMode::BEST : ExitGames::LoadBalancing::RegionSelectionMode::SELECT;
#else
static const nByte regionSelectionMode = ExitGames::LoadBalancing::RegionSelectionMode::SELECT;
#endif

static const EG_CHAR* PLAYER_NAME =
#if defined _EG_WINDOWS_PLATFORM
L"Windows";
#elif defined _EG_IPHONE_PLATFORM
L"iOS";
#elif defined _EG_IMAC_PLATFORM
L"OS X";
#elif defined _EG_MARMALADE_PLATFORM
L"Marmalade";
#elif defined _EG_ANDROID_PLATFORM
L"Android";
#elif defined _EG_PS3_PLATFORM
L"PS3";
#elif defined _EG_LINUX_PLATFORM
L"Linux";
#elif defined _EG_PS4_PLATFORM
L"PS4";
#elif defined _EG_WINDOWSSTORE_PLATFORM
L"Windows Store";
#elif defined _EG_EMSCRIPTEN_PLATFORM
L"Emscripten";
#elif defined _EG_PSVITA_PLATFORM
L"PS Vita";
#elif defined _EG_XB1_PLATFORM
L"XB1";
#elif defined _EG_SWITCH_PLATFORM
L"Switch";
#else
L"unknown platform";
#endif

ExitGames::Common::JString& NetworkLogicListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
{
	return retStr;
}

LOAD_STATE StateAccessor::getState(void) const
{
	return mState;
}

void StateAccessor::setState(LOAD_STATE newState)
{
	mState = newState;
	for (unsigned int i = 0; i < mStateUpdateListeners.getSize(); i++)
		mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateUpdateListeners.addElement(listener);
}



Input NetworkLogic::getLastInput(void) const
{
	return mLastInput;
}

void NetworkLogic::setLastInput(Input newInput)
{
	mLastInput = newInput;
}

LOAD_STATE NetworkLogic::getState(void) const
{
	return mStateAccessor.getState();
}

#ifdef _EG_XB1_PLATFORM
void NetworkLogic::setXSTSToken(const ExitGames::Common::JVector<nByte>& XSTSToken)
{
	if (mXSTSToken == XSTSToken)
		return;
	mXSTSToken = XSTSToken;
	mReauthenticateRequired = true;
}
#elif defined _EG_SWITCH_NX_PLATFORM
void NetworkLogic::setIDToken(const ExitGames::Common::JString& IDToken)
{
	mIDToken = IDToken;
}
#endif

// functions
NetworkLogic::NetworkLogic(OutputListener* listener, const EG_CHAR* appID)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
	: mLoadBalancingClient(*this, appID, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, autoLobbyStats, regionSelectionMode)
	, mLastInput(INPUT_NON)
	, mpOutputListener(listener)
#if defined _EG_EMSCRIPTEN_PLATFORM || defined _EG_PSVITA_PLATFORM || defined _EG_XB1_PLATFORM || defined _EG_SWITCH_PLATFORM
	, mAutoJoinRoom(true)
#else
	, mAutoJoinRoom(false)
#endif
	, mLastSendTime(GETTIMEMS())
#ifdef _EG_XB1_PLATFORM
	, mReauthenticateRequired(false)
#endif
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
	mStateAccessor.setState(STATE_INITIALIZED);
	mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
	mLogger.setListener(*this);
	mLogger.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // this class
	ExitGames::Common::Base::setListener(this);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
	_joiningPeer = 0;
}

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::connect(void)
{
	mpOutputListener->writeLine(ExitGames::Common::JString(L"connecting to Photon"));
	ExitGames::LoadBalancing::AuthenticationValues authValues;
#ifdef _EG_XB1_PLATFORM
	authValues.setType(ExitGames::LoadBalancing::CustomAuthenticationType::XBOX).setData(mXSTSToken);
	mReauthenticateRequired = false;
#elif defined _EG_SWITCH_NX_PLATFORM
	authValues.setType(ExitGames::LoadBalancing::CustomAuthenticationType::NINTENDO_SWITCH).setParameters(L"token=" + mIDToken);
#elif defined _EG_PS4_PLATFORM
	authValues.setType(ExitGames::LoadBalancing::CustomAuthenticationType::PLAYSTATION).setParameters(L"token=" + AuthenticationParameters::get().authCode + L"&env=" + AuthenticationParameters::get().env + L"&userName=" + AuthenticationParameters::get().userID);
#else // we could still set set the userID when doing console authentication, but it would be pointless, as the server would override it with the userID that is set by the auth-provider, anyway. Hence we only set it in the #else case
	if (mUserID.length()) // if we don't have a user Id yet, let the server assign one for us, afterwards store the assigned value in mUserID in connectReturn() and re-use it in subsequent connects, so that Photon recognizes us as the same user
		authValues.setUserID(mUserID);
#endif
	// connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
	if(!mLoadBalancingClient.connect(authValues, PLAYER_NAME))
//	if (!mLoadBalancingClient.connect())
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
	mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect(); // disconnect() is asynchronous - the actual result arrives in the Listener::disconnectReturn() callback
}

void NetworkLogic::opCreateRoom(const ExitGames::Common::JString& roomName, int maxPlayers, nByte directMode)
{
	mLoadBalancingClient.opCreateRoom(roomName, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers).setPlayerTtl(INT_MAX / 2).setEmptyRoomTtl(10000).setDirectMode(directMode));
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"creating room") + L"...");
}

void NetworkLogic::opJoinOrCreateRoom(void)
{
	ExitGames::Common::JString roomID(L"DemoLoadBalancing");
	mLoadBalancingClient.opJoinOrCreateRoom(roomID);
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"joining or creating room ") + roomID + L"...");
}

void NetworkLogic::opJoinRandomOrCreateRoom(void)
{
	mLoadBalancingClient.opJoinRandomOrCreateRoom();
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"joining a random room or creating a new room") + L"...");
}

void NetworkLogic::opJoinRoom(const ExitGames::Common::JString& roomID, bool rejoin)
{
	mLoadBalancingClient.opJoinRoom(roomID, rejoin);
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(rejoin?L"re":L"") + L"joining room " + roomID + "...");
}

void NetworkLogic::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"joining random room..."));
}

void NetworkLogic::run(void)
{
	LOAD_STATE state = mStateAccessor.getState();
	ExitGames::Common::EGTime t = GETTIMEMS();
	if (mLastInput == INPUT_EXIT && state != STATE_DISCONNECTING && state != STATE_DISCONNECTED)
	{
		disconnect();
		mStateAccessor.setState(STATE_DISCONNECTING);
		mpOutputListener->writeLine(L"terminating application");
	}
	else
	{
		switch (state)
		{
		case STATE_INITIALIZED:
			connect();
			break;
		case STATE_CONNECTING:
			break; // wait for callback
		case STATE_CONNECTED:
			switch (mLastInput)
			{
			case INPUT_1:
				mpOutputListener->writeLine(L"\n============= Create Game");
				opCreateRoom(L"None", 2, ExitGames::LoadBalancing::DirectMode::NONE);
				break;
			case INPUT_2:
				mpOutputListener->writeLine(L"\n============= Join Random Game");
				// remove false to enable rejoin
				if ((false) && mLastJoinedRoom.length())
					opJoinRoom(mLastJoinedRoom, true);
				else
					opJoinRandomRoom();
				break;
#ifndef _EG_EMSCRIPTEN_PLATFORM
			case INPUT_3:
				mpOutputListener->writeLine(L"\n============= Create Game Direct All To All");
				opCreateRoom(L"AllToAll", 4, ExitGames::LoadBalancing::DirectMode::ALL_TO_ALL);
				break;
			case INPUT_4:
				mpOutputListener->writeLine(L"\n============= Create Game Direct Master To All");
				opCreateRoom(L"MasterToAll", 4, ExitGames::LoadBalancing::DirectMode::MASTER_TO_ALL);
				break;
#endif
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_JOINING:
			break; // wait for callback
		case STATE_JOINED:
			if (mLastSendTime + sendInterval < t)
			{
				mLastSendTime = t;
				sendEvent();
			}
			switch (mLastInput)
			{
			case INPUT_1: // leave Game
				mLoadBalancingClient.opLeaveRoom();
				mpOutputListener->writeLine(L"");
				mpOutputListener->writeLine(L"leaving room");
				mStateAccessor.setState(STATE_LEAVING);
				break;
			case INPUT_2: // leave Game
				mLoadBalancingClient.opLeaveRoom(true);
				mpOutputListener->writeLine(L"");
				mpOutputListener->writeLine(L"leaving room (will come back)");
				mStateAccessor.setState(STATE_LEAVING);
				break;
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_LEAVING:
			break; // wait for callback
		case STATE_LEFT:
#ifdef _EG_XB1_PLATFORM
			if (mReauthenticateRequired)
			{
				EGLOG(ExitGames::Common::DebugLevel::INFO, L"reauthenticating");
				mStateAccessor.setState(STATE_DISCONNECTING);
				disconnect();
			}
			else
#endif
				mStateAccessor.setState(STATE_CONNECTED);
			break;
		case STATE_DISCONNECTING:
			break; // wait for callback
		default:
			break;
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service(); // needs to be called regularly!
}

void NetworkLogic::sendEvent(void)
{
	static int64 count = 0;
	if (mLoadBalancingClient.getCurrentlyJoinedRoom().getDirectMode() == ExitGames::LoadBalancing::DirectMode::NONE)
	{
		mLoadBalancingClient.opRaiseEvent(false, count, 0);
		mpOutputListener->write(ExitGames::Common::JString(L"S") + count + L" ");
	}
	else
		sendDirect(count);
	++count;
#if defined _EG_EMSCRIPTEN_PLATFORM
		mpOutputListener->writeLine(L"");
#endif
}

//@code : use distinct event codes to distinguish between different types of events (for example 'move', 'shoot', etc.)
//@eventContent : organize your payload data in any way you like as long as it is supported by Photons serialization
void NetworkLogic::sendEvent(nByte code, ExitGames::Common::Hashtable *eventContent)
{
 	bool sendReliable = true; // send something reliable if it has to arrive everywhere
	mLoadBalancingClient.opRaiseEvent(sendReliable, eventContent, 1, code);
}

void NetworkLogic::sendDirect(int64 count)
{
	int localNr = mLoadBalancingClient.getLocalPlayer().getNumber();
	ExitGames::Common::JString str = ExitGames::Common::JString(L"") + count + L"/p" + localNr;

	const int sendMode = 0;
	const bool fallbackRelay = true;
	switch (sendMode)
	{
	case 0: // send to all with one call
	{
		int sent = mLoadBalancingClient.sendDirect(str, ExitGames::Common::JVector<int>(), fallbackRelay);
		mpOutputListener->write(L"Sd" + str + L"->ALL(" + sent + ") ");
	}
	break;
	case 1: // send to a few with one call
	{
		ExitGames::Common::JVector<int> targets;
		for (int i = 0; i < mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayerCount() && targets.getSize() < 10; ++i)
		{
			int nr = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers()[i]->getNumber();
			if (nr % 2)
				targets.addElement(nr);
		}
		int sent = mLoadBalancingClient.sendDirect(str, targets, fallbackRelay);
		mpOutputListener->write(L"Sd" + str + L"->p" + targets.toString() + "(" + sent + ") ");
	}
	break;
	case 2: // send with call per player			
	{
		for (int i = 0; i < mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayerCount(); ++i)
		{
			int nr = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers()[i]->getNumber();
			if (nr != localNr)
			{
				if(mLoadBalancingClient.sendDirect(str, nr, fallbackRelay))
					mpOutputListener->write(L"Sd" + str + L"->p" + nr + L" ");
				else 
					mpOutputListener->write(L"Xd" + str + L"->p" + nr + L" ");
			}
		}
	}
	break;
	default:
		break;
	}
}

// protocol implementations

void NetworkLogic::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
	mpOutputListener->debugReturn(debugLevel, string);
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received connection error ") + errorCode);
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + " from server");
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");
	lpAppInfo.IncreasePeer();
}

void NetworkLogic::leaveRoomEventAction(int playerNr, bool isInactive)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + L" has left the game");
	lpAppInfo.DecreasePeer();
}

void NetworkLogic::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)

	// logging the string representation of the eventContent can be really useful for debugging, but use with care: for big events this might get expensive
	EGLOG(ExitGames::Common::DebugLevel::ALL, L"an event of type %d from player Nr %d with the following content has just arrived: %ls", eventCode, playerNr, eventContent.toString(true).cstr());

	mpOutputListener->write(ExitGames::Common::JString(L"R") + ExitGames::Common::ValueObject<long long>(eventContent).getDataCopy() + "<-p" + playerNr + L" ");
#if defined _EG_EMSCRIPTEN_PLATFORM
		mpOutputListener->writeLine(L"");
#endif
	
	ExitGames::Common::Hashtable* event;

	switch (eventCode) 
	{
		float x, y, z;
		bool isState, isHit;
	case 1:
		event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
		x = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
		y = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
		eventQueue.push({ static_cast<float>(playerNr), x, y });
		break;

	case 2:
		/// 座標情報を追加する
		event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
		x = ExitGames::Common::ValueObject<float>(event->getValue(0)).getDataCopy();
		y = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
		z = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
		bPosQueue.push({x, y, z });
		break;
	case 3:
		/// 速度情報を追加する
		event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
		x = ExitGames::Common::ValueObject<float>(event->getValue(0)).getDataCopy();
		y = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
		z = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
		bVelQueue.push({ x, y, z });
		break;
	case 4:
		/// 判定情報を追加する(修正するかも) ◆
		event  = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
		isState = ExitGames::Common::ValueObject<bool>(event->getValue(0)).getDataCopy();
		isHit = ExitGames::Common::ValueObject<bool>(event->getValue(1)).getDataCopy();
		isQueue.push({ isState, isHit });
		break;
	default:
		break;
	}
}

void NetworkLogic::onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay)
{
	mpOutputListener->write(ExitGames::Common::JString(L"Rd") + (relay?L"(R)":L"") + msg.toString() + "<-p" + remoteID + L" ");
}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"connected to cluster " + cluster + L" of region " + region + L", user id = " + mLoadBalancingClient.getUserID());
	mpOutputListener->writeLine(L"connected to cluster " + cluster + L" of region " + region + L", user id = " + mLoadBalancingClient.getUserID());
	mUserID = mLoadBalancingClient.getUserID();
	mStateAccessor.setState(STATE_CONNECTED);
	if (mAutoJoinRoom)
		opJoinOrCreateRoom();
}

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"disconnected");
#ifdef _EG_XB1_PLATFORM
	mStateAccessor.setState(mReauthenticateRequired ? STATE_INITIALIZED : STATE_DISCONNECTED);
#else
	mStateAccessor.setState(STATE_DISCONNECTED);
#endif
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been created");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);

	// ルーム内で割り当てられたプレイヤー番号を取得する
	playerNr = localPlayerNr;
}

void NetworkLogic::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinOrCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomOrCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		mLastJoinedRoom = L"";
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);

	// ルーム内で割り当てられたプレイヤー番号を取得する
	playerNr = localPlayerNr;
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}

	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);

	// ルーム内で割り当てられたプレイヤー番号を取得する
	playerNr = localPlayerNr;
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opLeaveRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_LEFT);
	mpOutputListener->writeLine(L"room has been successfully left");
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"joined lobby");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"left lobby");
}

void NetworkLogic::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls", lobbyStats.toString().cstr());
	mpOutputListener->writeLine(L"LobbyStats: " + lobbyStats.toString());
}

void NetworkLogic::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls", lobbyStats.toString().cstr());
	mpOutputListener->writeLine(L"LobbyStats: " + lobbyStats.toString());
}

void NetworkLogic::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls / %ls", availableRegions.toString().cstr(), availableRegionServers.toString().cstr());
	mpOutputListener->writeLine(L"onAvailableRegions: " + availableRegions.toString() + L" / " + availableRegionServers.toString());
	// select first region from list
	ExitGames::Common::JString selectedRegion;
	mpOutputListener->writeLine(L"selecting region: " + (selectedRegion=availableRegions[0]));
	mLoadBalancingClient.selectRegion(selectedRegion);
}

bool NetworkLogic::isRoomExists(void)
{
	if (mLoadBalancingClient.getRoomList().getIsEmpty()) {
		return false;
	}

	return true;
}
