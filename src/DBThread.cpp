

#include <iostream>
#include "DBThread.h"

using namespace std;



    DBThread::DBThread() :
        shutdown(false),
        pthread(0)
             {
    }

    DBThread::~DBThread() {
    }

    void *DBThread::runStatic(void *context){
        void *ret = ((DBThread *) context)->run();
        return ret;
    }

    void DBThread::stop(void) {
        shutdown = true;
    }

