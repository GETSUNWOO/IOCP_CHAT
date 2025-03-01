#pragma once

class GameSession;

using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

	vector<String> Get_players();

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
