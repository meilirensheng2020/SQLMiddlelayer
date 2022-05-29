

#include <string>
#include <pthread.h>


#ifndef DBTHREAD_H_
#define DBTHREAD_H_

using namespace std;



class DBThread {
public:
        volatile bool shutdown;
        pthread_t pthread;

        static void *runStatic(void *context);

        virtual void *run(void) = 0;
        virtual void stop(void);
        uint64_t initialize(void);

        DBThread();
        virtual ~DBThread();
    };


#endif
