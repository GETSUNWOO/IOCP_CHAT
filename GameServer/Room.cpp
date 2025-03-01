#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

shared_ptr<Room> GRoom = make_shared<Room>();

void Room::Enter(PlayerRef player)
{
	_players[player->name] = player;
}

void Room::Leave(PlayerRef player)
{
	_players.erase(player->name);
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for (auto& p : _players)
	{
		auto session = p.second->ownerSession.lock();
		if(session)
			session->Send(sendBuffer);
	}
}

void Room::BroadcastTest(SendBufferRef sendBuffer)
{
	for (auto& p : _players)
	{
		auto session = p.second->ownerSession.lock();
		if (session)
		{
			GConsoleLogger->WriteStdOut(Color::GREEN,
				L"조인 패킷 to [%s]", p.first.c_str());
			session->Send(sendBuffer);
		}
	}
}
