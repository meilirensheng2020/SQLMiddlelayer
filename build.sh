g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -g3 -Wall -c -fmessage-length=0 -MMD -MP -o ./src/Socket.o ./src/Socket.cpp 
g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -g3 -Wall -c -fmessage-length=0 -MMD -MP -o ./src/ServerSocket.o ./src/ServerSocket.cpp 
g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -g3 -Wall -c -fmessage-length=0 -MMD -MP -o ./src/DBThread.o ./src/DBThread.cpp 
g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -g3 -Wall -c -fmessage-length=0 -MMD -MP -o ./src/DBWriter.o ./src/DBWriter.cpp  -locilib
g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -pthread  ./src/SQLExecute.cpp -o SQLExecute -locilib  ./src/ServerSocket.o ./src/Socket.o ./src/DBThread.o ./src/DBWriter.o 
g++ -std=c++1y -I /home/yansp/SQLMiddlelayer -pthread  ./src/socketClient.cpp  -o  socketClient  
