#include "pch.h"
#include "ServerPacketHandler.h"
#include "Protocol.pb.h"
#include "FileUtils.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//TODO : log

	return false;
}

bool Handle_S_SIGNUP(PacketSessionRef& session, Protocol::S_SIGNUP& pkt)
{
    if (pkt.success() == false)
    {
        switch (pkt.error())    
        {
        case Protocol::ErrorCode::INVALID_NAME:
            MESSAGE->ShowPopup(u8"사용할 수 없는 아이디입니다");
            break;
        case Protocol::ErrorCode::INVALID_PASS:
            MESSAGE->ShowPopup(u8"사용할 수 없는 비밀번호입니다");
            break;
        case Protocol::ErrorCode::NAME_ALREADY_EXISTS:
            MESSAGE->ShowPopup(u8"이미 사용중인 아이디입니다");
            break;
        case Protocol::ErrorCode::UNKNOWN_ERROR:
            MESSAGE->ShowPopup(u8"UNKNOWN_ERROR");
            break;
        }
    }
    else
        MESSAGE->ShowPopup(u8"회원가입 되었습니다");

    return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
    if (pkt.success() == true)
    {
         SERVER->Set_State(STATE::CHAT);
         vector<string> players;
         int32 size = pkt.players_size();
         for (int i = 0; i < size; i++)
         {
             players.push_back(pkt.players(i));
         }
         MESSAGE->FetchPlayer(players);
    }
    else
    {
        switch (pkt.error())
        {
        case Protocol::ErrorCode::INVALID_NAME:
            MESSAGE->ShowPopup(u8"사용할 수 없는 아이디입니다");
            break;
        case Protocol::ErrorCode::INVALID_PASS:
            MESSAGE->ShowPopup(u8"사용할 수 없는 비밀번호입니다");
            break;
        case Protocol::ErrorCode::UNKNOWN_ERROR:
            MESSAGE->ShowPopup(u8"UNKNOWN_ERROR");
            break;
        }
    }
    return true;
}

bool Handle_S_JOIN(PacketSessionRef& session, Protocol::S_JOIN& pkt)
{
    MESSAGE->AddPlayer(pkt.name());
    return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
    ChatMessage chat;
    chat.msg = pkt.msg();
    chat.name = pkt.name();
    chat.type = pkt.type();
    MESSAGE->AddMessage(chat);
	return true;
}

bool Handle_S_LEAVE(PacketSessionRef& session, Protocol::S_LEAVE& pkt)
{
    MESSAGE->ErasePlayer(pkt.name());
    return true;
}
