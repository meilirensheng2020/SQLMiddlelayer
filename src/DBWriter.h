
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include <pthread.h>
#include "DBThread.h"
#include "ocilib.hpp"
#include "ServerSocket.h"
#include "SocketException.h"

#ifndef DBWRITER_H_
#define DBWRITER_H_

using namespace std;
using namespace ocilib;


    class DBWriter : public DBThread {


    public:

        Pool pool;
       // ServerSocket mainSocket;
        ServerSocket dbSocket;
        //string sql;
        //DBWriter(Pool pool,ServerSocket mainSocket);
        DBWriter(Pool pool);
        ~DBWriter();
        void *run(void);
        //int initOracleEnv();

    };

#endif
