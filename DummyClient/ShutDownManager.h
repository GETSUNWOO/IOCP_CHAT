#pragma once
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "DBConnectionPool.h"
#include "ConsoleLog.h"

/*----------------
    ShutDownManager
------------------*/

// 서버코어 자원들 winapi환경에서 지우기 위한

class ShutDownManager
{
    DECLARE_SINGLE(ShutDownManager);
private:
    bool _isShutdown = false;

public:
    void Shutdown()
    {
        if (_isShutdown)
            return;

        SERVER->Send_Leave();

        if (GDBConnectionPool)
        {
            delete GDBConnectionPool;
            GDBConnectionPool = nullptr;
        }

        if (GConsoleLogger)
        {
            delete GConsoleLogger;
            GConsoleLogger = nullptr;
        }


        // 1. 서버 매니저 정리 (세션, 네트워크 연결 정리)
        SERVER->Close();

        // 2. 스레드 정리
        if (GThreadManager)
        {
            delete GThreadManager;
            GThreadManager = nullptr;
        }

        // 3. SendBuffer 정리
        if (GSendBufferManager)
        {
            delete GSendBufferManager;
            GSendBufferManager = nullptr;
        }

        // 4. 네트워크 정리
        SocketUtils::Clear();

        // 5. 나머지 매니저 정리
        if (GMemory)
        {
            delete GMemory;
            GMemory = nullptr;
        }

        if (GDeadLockProfiler)
        {
            delete GDeadLockProfiler;
            GDeadLockProfiler = nullptr;
        }

        _isShutdown = true;
    }
};

