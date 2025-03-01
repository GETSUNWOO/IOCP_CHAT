#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

void ServerSession::OnConnected()
{
	SERVER->Set_Connect();
}

void ServerSession::OnDisconnected()
{
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{
}
