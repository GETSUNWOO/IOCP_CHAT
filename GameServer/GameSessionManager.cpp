#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "Player.h"
GameSessionManager GSessionManager;

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}

vector<String> GameSessionManager::Get_players()
{
	WRITE_LOCK;
	vector<String> out;
	for (GameSessionRef session : _sessions)
	{
		if (session->_player == nullptr) continue;
		if (session->_player->type == Protocol::PLAYERTYPE_USER)
		{
			out.push_back(session->_player->name);
		}
	}

	return out;
}
