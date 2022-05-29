#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <errno.h>
#include "ocilib.hpp"

using namespace ocilib;
using namespace std;

const int MaxThreads = 50;
const int MaxConnnections = 100;

void worker(ThreadHandle handle, void *data)
{
    Connection con = ((Pool *)data)->GetConnection();
    Statement st(con);
    st.Execute("select to_char(sysdate, 'YYYYMMDD HH24:MI:SS') from dual");
    Resultset rs = st.GetResultset();
    rs.Next();
    std::cout << handle << " - " << rs.Get<ostring>(1) << std::endl;
    sleep(10);
    con.Close();
    sleep(60);
}

int main(void)
{
    try
    {
        Environment::Initialize(Environment::Threaded);

        Pool pool("192.168.1.88/yunhuandb", "dbmgr", "yanshoupeng", Pool::ConnectionPool, 10,MaxConnnections,10);


        struct sockaddr_in  servaddr;
        int listenfd,connfd;


        if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){

            cout << "Error:create listener socket failed: "<< strerror(errno) <<"(errno:"<< errno <<")"<< endl;
            return -1;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(1234);

        if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
            cout << "Error:bind  socket failed: "<< strerror(errno) <<"(errno:"<< errno <<")"<< endl;
            return -1;
        }

        if( listen(listenfd, 10) == -1){
            cout << "listen socket failed: "<< strerror(errno) <<"(errno:"<< errno <<")"<< endl;
            return -1;
        }

        cout << "- listen on port:  " << 1234 << endl;



        while(1){

            char cmdBuffer[8192];
            
            memset(cmdBuffer, 0, sizeof(cmdBuffer));
            if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
                cout << "Error:accept socket failed: "<< strerror(errno) <<"(errno:"<< errno <<")"<< endl;
                continue;
            }
            int numBytes = recv(connfd, cmdBuffer, 8192, 0);
            cmdBuffer[numBytes] = '\0';
            printf("- recv message from client: %s\n", cmdBuffer);
            if (numBytes>0) 
            { 

                try   
                {                   

                    //string returnMsg="operate success";

                    //if( send(connfd, returnMsg.c_str(), returnMsg.length(), 0) < 0){
                        //printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                       
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }

            }
            
            close(connfd);
        }


       /* std::vector<ThreadHandle> threads;

        for (int i = 0; i < MaxThreads; i++)
        {
            ThreadHandle th = Thread::Create();
            threads.push_back(th);
            Thread::Run(th, worker, &pool);
        }

        for (int i = 0; i < MaxThreads; i++)
        {
            Thread::Join(threads[i]);
            Thread::Destroy(threads[i]);
        }*/
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    Environment::Cleanup();

    return EXIT_SUCCESS;
}