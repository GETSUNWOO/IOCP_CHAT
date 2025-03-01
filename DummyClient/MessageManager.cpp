#include "pch.h"
#include "MessageManager.h"

void MessageManager::Init()
{
}

void MessageManager::Close()
{
    _messageDeque.clear();
}

void MessageManager::AddMessage(const ChatMessage& msg)
{
    WRITE_LOCK;
    _messageDeque.push_back(msg);
    if (_messageDeque.size() > MAX_MESSAGES)
    {
        _messageDeque.pop_front();
    }
}

void MessageManager::FetchPlayer(const vector<string>& players)
{
    WRITE_LOCK;
    _players = players;
}

void MessageManager::AddPlayer(const string& name)
{
    WRITE_LOCK;
    _players.push_back(name);
}

void MessageManager::ErasePlayer(const string& name)
{
    WRITE_LOCK;
    auto it = find(_players.begin(), _players.end(), name);
    if (it != _players.end()) _players.erase(it);
}

vector<ChatMessage> MessageManager::GetRecentMessages()
{
    WRITE_LOCK;
    return vector<ChatMessage>(_messageDeque.begin(), _messageDeque.end());
}

vector<string> MessageManager::GetPlayers()
{
    WRITE_LOCK;
    return _players;
}

void MessageManager::ShowPopup(string msg)
{
    WRITE_LOCK;
    if (!_isPopup)
    {
        _isPopup.store(true);
        _popupMessage = msg;
    }
}

const string MessageManager::GetPopupMessage()
{
    WRITE_LOCK;
    if (_isPopup)
    	return _popupMessage;
    
    return string();
}

void MessageManager::ClearPopup()
{
    WRITE_LOCK;
    if (_isPopup)
    {
    	_isPopup.store(false);
    	_popupMessage.clear();
    }
}