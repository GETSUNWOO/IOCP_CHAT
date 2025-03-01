#pragma once
#include "JobQueue.h"
class Room : public JobQueue
{
public: // 싱글스레드인것처럼 작동
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);
	void BroadcastTest(SendBufferRef sendBuffer);

private:
	map<String, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;