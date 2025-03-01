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

// �����ھ� �ڿ��� winapiȯ�濡�� ����� ����

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


        // 1. ���� �Ŵ��� ���� (����, ��Ʈ��ũ ���� ����)
        SERVER->Close();

        // 2. ������ ����
        if (GThreadManager)
        {
            delete GThreadManager;
            GThreadManager = nullptr;
        }

        // 3. SendBuffer ����
        if (GSendBufferManager)
        {
            delete GSendBufferManager;
            GSendBufferManager = nullptr;
        }

        // 4. ��Ʈ��ũ ����
        SocketUtils::Clear();

        // 5. ������ �Ŵ��� ����
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

