#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "Player.h"
#include "Room.h"
#include "ServerGlobal.h"
#include "DBConnectionPool.h"
#include "GenProcedures.h"
#include "FileUtils.h"
#include <ctime>
#include "GameSessionManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//TODO : log

	return false;
}

bool Handle_C_SIGNUP(PacketSessionRef& session, Protocol::C_SIGNUP& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_SIGNUP signupPkt;

	//string 변환
	String u16name = FileUtils::Convert(pkt.name());
	const WCHAR* name = u16name.c_str();
	int32 namecnt = static_cast<int32>(u16name.length());

	String u16password = FileUtils::Convert(pkt.password());
	const WCHAR* password = u16password.c_str();
	int32 passwordcnt = static_cast<int32>(u16password.length());  // 길이를 계산

	//현재시간
	std::time_t t = std::time(nullptr);
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	
	localtime_s(&tm, &t);
	TIMESTAMP_STRUCT timestamp = {};
	timestamp.year = static_cast<short>(tm.tm_year + 1900);
	timestamp.month = static_cast<short>(tm.tm_mon + 1);
	timestamp.day = static_cast<short>(tm.tm_mday);
	timestamp.hour = static_cast<short>(tm.tm_hour);
	timestamp.minute = static_cast<short>(tm.tm_min);
	timestamp.second = static_cast<short>(tm.tm_sec);
	timestamp.fraction = 0;

	//validation 체크

	DBConnection* dbConn = GDBConnectionPool->Pop();
	SP::GetAccount GetAccount(*dbConn);
	GetAccount.In_Name(name, namecnt);
	int32 outId;
	int32 outType;
	WCHAR outName[100];
	WCHAR outPassword[100];
	TIMESTAMP_STRUCT outCreateDate;
	GetAccount.Out_Id(OUT outId);
	GetAccount.Out_Type(OUT outType);
	GetAccount.Out_Name(OUT outName);
	GetAccount.Out_Password(OUT outPassword);
	GetAccount.Out_CreateDate(OUT outCreateDate);
	GetAccount.Execute();

	if (GetAccount.Fetch())
	{
		signupPkt.set_success(false);
		signupPkt.set_error(Protocol::ErrorCode::NAME_ALREADY_EXISTS);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(signupPkt);
		gameSession->Send(sendBuffer);
		GConsoleLogger->WriteStdOut(Color::RED, L"회원가입 거부 : NAME_ALREADY_EXISTS[%s]\n", name);
		GDBConnectionPool->Push(dbConn); //db반환
		return true;
	}

	if (u16name.size() > GMAXNAMELEN || u16name.size() < GMINNAMELEN)
	{
		signupPkt.set_success(false);
		signupPkt.set_error(Protocol::ErrorCode::INVALID_NAME);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(signupPkt);
		gameSession->Send(sendBuffer);
		GConsoleLogger->WriteStdOut(Color::RED,
			L"회원가입 거부 : INVALID_NAME[%s]\n", name);

		GDBConnectionPool->Push(dbConn); //db반환
		return true;
	}

	if (u16password.size() > GMAXPASSWORDLEN || u16password.size() < GMINPASSWORDLEN)
	{
		signupPkt.set_success(false);
		signupPkt.set_error(Protocol::ErrorCode::INVALID_PASS);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(signupPkt);
		gameSession->Send(sendBuffer);
		GConsoleLogger->WriteStdOut(Color::RED,
			L"회원가입 거부 : INVALID_PASS[%s]\n", password);

		GDBConnectionPool->Push(dbConn); //db반환
		return true;
	}

	//DB에 정보를 입력한다.
	SP::InsertAccount insertAccount(*dbConn);
	insertAccount.In_Type(static_cast<int>(Protocol::PlayerType::PLAYERTYPE_USER));
	insertAccount.In_Name(name, namecnt);
	insertAccount.In_Password(password, passwordcnt);
	insertAccount.In_CreateDate(timestamp);
	insertAccount.Execute();

	GDBConnectionPool->Push(dbConn); //db반환

	//패킷을 조립한다
	signupPkt.set_success(true);

	//log
	GConsoleLogger->WriteStdOut(Color::BLUE,
		L"회원가입 승인 : Name[%s] Password[%s] Date[%04d-%02d-%02d %02d:%02d:%02d]\n", name, password, timestamp.year, timestamp.month, timestamp.day, timestamp.hour, timestamp.minute,timestamp.second);

	return true;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_LOGIN loginPkt;
	//string 변환
	String u16name = FileUtils::Convert(pkt.name());
	const WCHAR* name = u16name.c_str();
	int32 namecnt = static_cast<int32>(u16name.length());

	String u16password = FileUtils::Convert(pkt.password());
	const WCHAR* password = u16password.c_str();
	int32 passwordcnt = static_cast<int32>(u16password.length());  // 길이를 계산

	//현재시간
	std::time_t t = std::time(nullptr);
	struct tm tm;
	localtime_s(&tm, &t);
	TIMESTAMP_STRUCT timestamp = {};
	timestamp.year = static_cast<short>(tm.tm_year + 1900);
	timestamp.month = static_cast<short>(tm.tm_mon + 1);
	timestamp.day = static_cast<short>(tm.tm_mday);
	timestamp.hour = static_cast<short>(tm.tm_hour);
	timestamp.minute = static_cast<short>(tm.tm_min);
	timestamp.second = static_cast<short>(tm.tm_sec);
	timestamp.fraction = 0;

	//validation
	DBConnection* dbConn = GDBConnectionPool->Pop();
	SP::GetAccount GetAccount(*dbConn);
	GetAccount.In_Name(name, namecnt);
	int32 outId;
	int32 outType;
	WCHAR outName[100];
	WCHAR outPassword[100];
	TIMESTAMP_STRUCT outCreateDate;
	GetAccount.Out_Id(OUT outId);
	GetAccount.Out_Type(OUT outType);
	GetAccount.Out_Name(OUT outName);
	GetAccount.Out_Password(OUT outPassword);
	GetAccount.Out_CreateDate(OUT outCreateDate);
	GetAccount.Execute();

	//name check
	if (!GetAccount.Fetch())
	{
		loginPkt.set_success(false);
		loginPkt.set_error(Protocol::ErrorCode::INVALID_NAME);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
		gameSession->Send(sendBuffer);
		GConsoleLogger->WriteStdOut(Color::RED, L"로그인 거부 : INVALID_NAME[%s] \n", name);
		GDBConnectionPool->Push(dbConn); //db반환
		return true;
	}

	//password check
	if (wcscmp(outPassword, password) != 0)
	{
		loginPkt.set_success(false);
		loginPkt.set_error(Protocol::ErrorCode::INVALID_PASS);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
		gameSession->Send(sendBuffer);
		GConsoleLogger->WriteStdOut(Color::RED,
			L"로그인 거부 : INVALID_PASSWORD[%s] \n", password);

		GDBConnectionPool->Push(dbConn); //db반환
		return true;
	}
	GDBConnectionPool->Push(dbConn); //db반환
	
	//Room에 저장할 플레이어 생성
	PlayerRef player = MakeShared<Player>();
	player->id = outId;
	player->name = name;
	player->ownerSession = gameSession;
	player->type = static_cast<Protocol::PlayerType>(outType);
	gameSession->_player = player;

	//조인 패킷 입력 : 신입플레이어의 이름을 기존 플레이어에게 전달 (관리자는 제외)
	if (player->type == Protocol::PlayerType::PLAYERTYPE_USER)
	{
		Protocol::S_JOIN joinPkt;
		joinPkt.set_name(pkt.name());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(joinPkt);
		GRoom->DoAsync(&Room::BroadcastTest, sendBuffer);
		GConsoleLogger->WriteStdOut(Color::GREEN,
			L"from [%s]\n", name);
	}

	//Room에 등록
	GRoom->DoAsync(&Room::Enter, gameSession->_player); // WRITE_LOCK

	//로그인 패킷 입력
	loginPkt.set_success(true);
	vector<String> players = GSessionManager.Get_players(); // WRITE_LOCK
	//players.push_back(name);
	for (auto p : players)
	{
		loginPkt.add_players(FileUtils::Convert(p));
	}

	{
		// 로그인 패킷 전송
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
		gameSession->Send(sendBuffer);
	}

	GConsoleLogger->WriteStdOut(Color::GREEN,
		L"로그인 : [%s] \n", name);

	return true;
}

bool Handle_C_JOIN(PacketSessionRef& session, Protocol::C_JOIN& pkt)
{
	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	chatPkt.set_type(static_cast<Protocol::PlayerType>(gameSession->_player->type));
	chatPkt.set_name(FileUtils::Convert(gameSession->_player->name));

	GConsoleLogger->WriteStdOut(Color::RED,
		L"[%s] : [%s] \n", FileUtils::Convert(chatPkt.name()).c_str(), FileUtils::Convert(chatPkt.msg()).c_str());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
	GRoom->DoAsync(&Room::Broadcast,sendBuffer); // WRITE_LOCK

	return true;
}

bool Handle_C_LEAVE(PacketSessionRef& session, Protocol::C_LEAVE& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	Protocol::S_LEAVE leavePkt;
	leavePkt.set_name(FileUtils::Convert(gameSession->_player->name));
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leavePkt);
	GRoom->DoAsync(&Room::Broadcast, sendBuffer); // WRITE_LOCK

	return true;
}