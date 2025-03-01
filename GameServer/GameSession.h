#pragma once
#include "Session.h"

class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:

	// 다중 케릭터를 사용하는 게임
	//Vector<PlayerRef>		_players;

	PlayerRef				_player;
	weak_ptr<class Room>	_room;
};