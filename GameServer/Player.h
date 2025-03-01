#pragma once

class Player
{
public:
	int32					id;
	String					name;
	Protocol::PlayerType	type = Protocol::PlayerType::PLAYERTYPE_NONE;
	weak_ptr<GameSession>	ownerSession;
};

