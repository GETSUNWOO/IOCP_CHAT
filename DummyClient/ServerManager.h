#pragma once

/*----------------
	ServerManager
------------------*/

// 1. ���� �ʱ�ȭ , 2. ��Ŷ ���� �Լ�

class ServerManager
{
	DECLARE_SINGLE(ServerManager);

public:
	void Init();

	void Close();

	~ServerManager();

public:
	void Send_SignUp(string id, string password);
	void Send_Login(string id, string password);
	void Send_Chat(string chat);
	void Send_Leave();

//Setter
public:
	void Set_State(STATE state) { _state = state; }
	void Set_Connect() { _isConnected = true; }

//Getter
public:
	const STATE Get_State() { return _state; }
	const bool  Get_Connect() { return _isConnected; }

private:
	STATE _state = STATE::LOGIN;

private:
	bool _isConnected = false;
	Atomic<bool> _running = false;

private:
	ClientServiceRef _service;
};