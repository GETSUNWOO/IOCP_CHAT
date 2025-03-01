#pragma once
#include "JobQueue.h"
class Room : public JobQueue
{
public: // �̱۽������ΰ�ó�� �۵�
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);
	void BroadcastTest(SendBufferRef sendBuffer);

private:
	map<String, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;