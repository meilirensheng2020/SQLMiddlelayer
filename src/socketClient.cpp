#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <string>
#define MAXLINE 4096

using namespace std;

int main(int argc, char** argv){
    int   sockfd, n;
    char  recvline[4096], sendline[4096];
    struct sockaddr_in  servaddr;

    if( argc != 4){
        printf("usage: ./client <ipaddress> <port>  <nums>\n");
        return 0;
    }

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }


            // 接收缓冲区
    int nRecvBuf=32*1024;         //设置为32K
    setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

    //发送缓冲区
    int nSendBuf=32*1024;//设置为32K
    setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));


    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",argv[1]);
        return 0;
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    //while (true)

    int nums=atoi(argv[3]);

    //string sqltxt="BEGIN INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL); COMMIT; END;";
    string sqltxt = "BEGIN INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL); INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);INSERT INTO TT01  VALUES ('SYS', 'I_COLTYPE2', NULL, '85', '85', 'INDEX', TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-08-24 11:37:36', 'YYYY-MM-DD HH24:MI:SS'), 'VALID', 'N', 'N', 'N', '4', NULL);COMMIT; END;";
    string sql;
    for (int i=1;i<=nums;i++)
    {
        //printf("send msg to server: \n");
       // fgets(sendline, 4096, stdin);
       
        /*if (sql.length()+sqltxt.length()>=4096) {

            strcpy(sendline,sql.c_str());
            if( send(sockfd, sendline, strlen(sendline), 0) < 0){
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                return 0;
            }
            int numBytes = recv(sockfd, recvline, MAXLINE, 0);
            printf("recv msg from server:%s",recvline);
            sql.assign("");

        } else
        {
            sql = sql + sqltxt;
        }*/


        strcpy(sendline,sqltxt.c_str());
        if( send(sockfd, sendline, strlen(sendline), 0) < 0){
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 0;
        }
        int numBytes = recv(sockfd, recvline, MAXLINE, 0);
        printf("recv msg from server:%s",recvline);
        

    }

    
    strcpy(sendline,"commit");

    if( send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    int numBytes = recv(sockfd, recvline, MAXLINE, 0);
    printf("recv msg from server:%s",recvline);


    

    close(sockfd);
    return 0;
}