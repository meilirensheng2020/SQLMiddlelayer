#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <errno.h>
#include "ServerSocket.h"
#include "SocketException.h"
#include "ocilib.hpp"
#include "DBWriter.h"
#include <rapidjson/document.h>
#include <sys/file.h>
#include <sys/stat.h>

using namespace ocilib;
using namespace std;
using namespace rapidjson;


const int MaxThreads = 50;
const int MaxConnnections = 100;

const Value& getJSONfieldV(string &fileName, const Value& value, const char* field) {
    if (!value.HasMember(field)) {
        cerr << "parsing " << fileName << ", field " << field << " not found" << endl;
    }
    return value[field];
}

const Value& getJSONfieldD(string &fileName, const Document& document, const char* field) {
    if (!document.HasMember(field)) {
        cerr << "parsing " << fileName << ", field " << field << " not found" << endl;
    }
    return document[field];
}

int main(void)
{

    char *configFileBuffer = nullptr;
    struct stat fileStat;
    string configFileName = "SQLExecute.json";
    int fid = open(configFileName.c_str(), O_RDONLY);
    if (fid == -1) {
        cerr << "can't open file " << configFileName << endl;
        exit(1);
    }

    if (flock(fid, LOCK_EX | LOCK_NB)) {
        cerr << "can't lock file " << configFileName << ", another process may be running" << endl;
        exit(1);
    }

    int ret = stat(configFileName.c_str(), &fileStat);
    if (ret != 0) {
        cerr << "can't check file size of " << configFileName << endl;
        exit(1);
    }
    if (fileStat.st_size == 0) {
        cerr <<  "file " << configFileName << " is empty" << endl;
        exit(1);
    }

    configFileBuffer = new char[fileStat.st_size + 1];
    if (configFileBuffer == nullptr) {
        cerr << "couldn't allocate " << dec << (fileStat.st_size + 1) << " bytes memory (for: reading " << configFileName << ")" << endl;
    }
    if (read(fid, configFileBuffer, fileStat.st_size) != fileStat.st_size) {
        cerr <<  "can't read file " << configFileName << endl;
        exit(1);
    }
    configFileBuffer[fileStat.st_size] = 0;

    Document document;
    if (document.Parse(configFileBuffer).HasParseError()) {
        cerr <<  "parsing SQLExecute.json" << endl;
        exit(1);
    }

    /* const Value& versionJSON = getJSONfieldD(configFileName, document, "version");
    if (strcmp(versionJSON.GetString(), PACKAGE_VERSION) != 0) {
        CONFIG_FAIL("bad JSON, incompatible \"version\" value, expected: " << PACKAGE_VERSION << ", got: " << versionJSON.GetString());
    }*/

    //optional
    uint64_t port = 0;
    if (document.HasMember("port")) {
        const Value& dumpPortJSON = document["port"];
        port = dumpPortJSON.GetUint64();
    }


    const Value& dumpUsernameJSON = document["username"];
    string username  = dumpUsernameJSON.GetString();

    const Value& dumpPasswordJSON = document["password"];
    string password  = dumpPasswordJSON.GetString();

    const Value& dumpServerJSON = document["server"];
    string server  = dumpServerJSON.GetString();

        

    try
    {
        Environment::Initialize(Environment::Threaded);

        Pool pool(server, username, password, Pool::ConnectionPool,10,MaxConnnections,10);

        try
        {
            // Create the socket
            ServerSocket server ( port );
            cout << "- Runing on port " << port << endl;

            while ( true )
            {

                //ServerSocket new_sock;
                DBWriter *dbwriter = new  DBWriter(pool);
                server.accept ( dbwriter->dbSocket );
                pthread_create(&dbwriter->pthread, nullptr, &DBWriter::runStatic, (void*)dbwriter);
                pthread_detach(dbwriter->pthread);

            }
        }
         catch ( SocketException& e )
        {
            std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
        }

    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    Environment::Cleanup();

    return EXIT_SUCCESS;
}