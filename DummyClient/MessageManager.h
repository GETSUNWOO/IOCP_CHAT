#pragma once
#include "Protocol.pb.h"
/*----------------
	MessageManager
------------------*/

// 1. ä�� ��� ���� , 2. �˾�â �˸�

struct ChatMessage
{
	string name;
	string msg;
	Protocol::PlayerType type;
};

class MessageManager
{
	DECLARE_SINGLE(MessageManager);

public:
	void Init();
	void Close();

public:
	void AddMessage(const ChatMessage& msg);
	void FetchPlayer(const vector<string>& players);
	void AddPlayer(const string& name);
	void ErasePlayer(const string& name);

	vector<ChatMessage> GetRecentMessages();
	vector<string> GetPlayers();

public:
	void ShowPopup(string msg);
	const string GetPopupMessage();
	void ClearPopup();

private:
	Deque<ChatMessage> _messageDeque;
	Atomic<bool>	_isPopup;
	string _popupMessage;  // ���� ǥ���� �˾� �޽���

	vector<string> _players;

private:
	USE_LOCK;
};
