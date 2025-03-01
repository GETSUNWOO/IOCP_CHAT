#include "pch.h"
#include "ServerManager.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

void ServerManager::Init()
{
	_running = true;

	ServerPacketHandler::Init();

	_service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1);

	if (!_service || !_service->Start()) 
	{
		throw std::runtime_error("Service initialization failed");
	}

	for (int32 i = 0; i < 1; i++) 
	{
		GThreadManager->Launch([this]() 
			{
				while (_running && _service && _service->GetIocpCore()) 
				{
					_service->GetIocpCore()->Dispatch(100);  // 100ms timeout
				}
			});
	}
}

void ServerManager::Close()
{
	// 먼저 running 플래그를 false로 설정하여 새로운 작업 중단
	_running = false;

	// ThreadManager 정리
	if (GThreadManager)
	{
		GThreadManager->Join();
	}

	// 다른 리소스들 정리
	_isConnected = false;
	_state = STATE::LOGIN;

	// Service 정리
	if (_service) 
	{
		_service->CloseService();

		if (_service->GetIocpCore()) 
		{
			PostQueuedCompletionStatus(_service->GetIocpCore()->GetHandle(), 0, 0, nullptr);
		}
	}

	if (_service.use_count() == 1) 
	{
		_service.reset();
	}
}

ServerManager::~ServerManager()
{
}

void ServerManager::Send_SignUp(string id, string password)
{
	Protocol::C_SIGNUP signUpPkt;
	signUpPkt.set_name(id);
	signUpPkt.set_password(password);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(signUpPkt);
	_service->Broadcast(sendBuffer);
}

void ServerManager::Send_Login(string id, string password)
{
	Protocol::C_LOGIN loginPkt;
	loginPkt.set_name(id);
	loginPkt.set_password(password);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPkt);
	_service->Broadcast(sendBuffer);
}

void ServerManager::Send_Chat(string chat)
{
	Protocol::C_CHAT ChatPkt;
	ChatPkt.set_msg(chat);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(ChatPkt);
	_service->Broadcast(sendBuffer);
}

void ServerManager::Send_Leave()
{
	Protocol::C_LEAVE LeavePkt;
	LeavePkt.set_error(Protocol::ErrorCode::DISCONNECT);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(LeavePkt);
	_service->Broadcast(sendBuffer);
}
