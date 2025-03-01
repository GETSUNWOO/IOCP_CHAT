#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;
		
		//네트워크 입출력 처리 -> 인게임 로직까지 (패킷핸들러)
		service->GetIocpCore()->Dispatch(10);

		//예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		//글로벌 큐
		ThreadManager::DoGlobalQueueWork();

		//이렇게 함으로써 네트워크 입출력처리와 인게임 로직을 모두 만능처리하여
		//어느 한곳에 일감을 적절히 분배해야하는 고민을 안해도 된다.
	}
}

int main()
{
	//DB ORM
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;"));

	DBConnection* dbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync(*dbConn);
	dbSync.Synchronize(L"GameDB.xml");
	GDBConnectionPool->Push(dbConn);

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				while (true)
				{
					DoWorkerJob(service);
				}				
			});
	}

	//Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}

/*{
	WCHAR name[] = L"Sunwoo";

	SP::InsertGold insertGold(*dbConn);
	insertGold.In_Gold(100);
	insertGold.In_Name(name);
	insertGold.In_CreateDate(TIMESTAMP_STRUCT{ 2020, 6, 8 });
	insertGold.Execute();
}

{
	SP::GetGold getGold(*dbConn);
	getGold.In_Gold(100);

	int32 id = 0;
	int32 gold = 0;
	WCHAR name[100];
	TIMESTAMP_STRUCT date;

	getGold.Out_Id(OUT id);
	getGold.Out_Gold(OUT gold);
	getGold.Out_Name(OUT name);
	getGold.Out_CreateDate(OUT date);

	getGold.Execute();

	while (getGold.Fetch())
	{
		GConsoleLogger->WriteStdOut(Color::BLUE,
			L"ID[%d] Gold[%d] Name[%s]\n", id, gold, name);
	}
}*/
