

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <sys/syscall.h>
#include "ocilib.hpp"
#include "DBWriter.h"
#include "SocketException.h"
#include <time.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


#define gettid() syscall(SYS_gettid)


using namespace std;
using namespace ocilib;
using namespace rapidjson;


DBWriter::DBWriter(Pool pool) :
    pool(pool)
    //mainSocket(mainSocket)
 {
    //initOracleEnv();

}

DBWriter::~DBWriter() {

    //dbSocket.close();

}

vector<string> split(const string &str,const string &pattern)
{
    //const char* convert to char*
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }

    delete[] strc;

    return resultVec;
}

void *DBWriter::run(void) {

    cout << "DBWriter created." << endl;

    Timestamp tm(Timestamp::NoTimeZone);
    tm = Timestamp::SysTimestamp();
    //std::cout << tm.ToString("YYYY-MM-DD HH24:MI:SS:FF3") << std::endl;

    Connection con = pool.GetConnection();
    Statement st(con);
    std::string data;

    while ( true)
    {
        
        try
        {

            data.assign("");
            dbSocket >> data;
           // cout << "length=" << data.length() << ",data=" << data << endl;
            cout << "length=" << data.length() << endl;

            if(data.compare("END\n")==0)  {
                cout << "Client closed" << endl;
                close(dbSocket.m_sock);
                break;
            }

            try {

                st.Execute(data);


                unsigned int rows = st.GetAffectedRows();

                StringBuffer s;
                Writer<StringBuffer> writer(s);

                writer.StartObject();

                writer.Key("retCode");                
                writer.String("00");
                writer.Key("retMsg");                
                writer.String("SUCCESS"); 
                writer.Key("rows");
                writer.Uint(rows); 
                //writer.Uint(1); 

                writer.EndObject();   
                string tmpdata = s.GetString() ;   
                tmpdata.append("\n"); 

                dbSocket << tmpdata ;
                
              //  Statement::StatementType  sqlType = st.GetStatementType();
                
                /*
                if (sqlType == Statement::TypeSelect) {

                    Resultset rs = st.GetResultset();
                    int numCols = rs.GetColumnCount();
                    string tmpdata;
                    while (rs++){

                        //tmpdata.clear();
                        for (int i = 1;i <= numCols; i++){

                            tmpdata.append(rs.Get<ostring>(i));
                            tmpdata.append(",");
                        }
                        tmpdata.pop_back();
                        tmpdata.append("\n");

                        if( (rs.GetCount() %10) ==0) {

                            dbSocket << tmpdata ;
                            tmpdata.clear();

                        } 

                        dbSocket << tmpdata ;
                        tmpdata.clear();

                    }

                    //string tmpdata = "{\"retCode\":\"00\",\"retMsg\":\"SUCCESS\"}\n";
                    
                }

                */



            /*    if (sqlType==Statement::TypeInsert || sqlType==Statement::TypeDelete || sqlType==Statement::TypeUpdate ) {

                    unsigned int rows = st.GetAffectedRows();

                    StringBuffer s;
                    Writer<StringBuffer> writer(s);

                    writer.StartObject();

                    writer.Key("retCode");                
                    writer.String("00");
                    writer.Key("retMsg");                
                    writer.String("SUCCESS"); 
                    writer.Key("rows");
                    writer.Uint(rows); 

                    writer.EndObject();   
                    string tmpdata = s.GetString() ;    

                    dbSocket << tmpdata ;
                    
                }

                */

               /*
                if (sqlType==Statement::TypeCreate || sqlType==Statement::TypeDrop || sqlType==Statement::TypeAlter ) {

                    StringBuffer s;
                    Writer<StringBuffer> writer(s);

                    writer.StartObject();

                    writer.Key("retCode");                
                    writer.String("00");
                    writer.Key("retMsg");                
                    writer.String("SUCCESS");  

                    writer.EndObject();   
                    string tmpdata = s.GetString() ;    

                    dbSocket << tmpdata ;
                }  else
                {
                    // code 
                }

                */

            
                


            }

            
            catch(const std::exception& e)
            {
                string errorMsg = e.what();
                errorMsg.pop_back();  //去掉换行符
                errorMsg = "{\"retCode\":\"01\",\"retMsg\":\"" + errorMsg + "\"}\n";
                //std::cerr << e.what() << '\n';
                dbSocket << errorMsg;
            
            }
            
        
        }
        catch ( SocketException& e )
        {
            std::cout << "Exception was caught:" << e.description() << std::endl;
            con.Rollback();
            break;  //Client abort connection
        }
           
    }

    con.Commit();
    con.Close();

    tm = Timestamp::SysTimestamp();
    //std::cout << tm.ToString("YYYY-MM-DD HH24:MI:SS:FF3") << std::endl;

    return EXIT_SUCCESS;
}


