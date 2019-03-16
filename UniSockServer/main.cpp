//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include "UniSocket.h"
#include "UniSocketSet.h"
#include <array>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room\r\n"

using std::string;
using std::array;

int main()
{
    UniSocket listenSock(5400, SOMAXCONN);

    UniSocketSet set;
    array<UniSocket, 2> ignoreSockets;
    set.addSock(listenSock);
    bool running = true;

    while(running)
    {
        int socketCount = set.select();
        for(int i = 0; i < socketCount;i++)
        {
            UniSocket currentSock = UniSocket(set.sockAt(i));
            if(listenSock == currentSock)
            {
                UniSocketStruct newClientStruct = listenSock.accept();
                UniSocket newClient = newClientStruct.data;
                if(newClientStruct.errorCode <= 0)
                    continue;
                set.addSock(newClient);
                newClient.send(WELCOME_MSG);
                LOG("Someone Has Joined!");
                array<UniSocket, 2> test = {newClient, listenSock};
                set.broadcast("Someone Has Joined!\r\n", test);
            } else
            {
                UniSocketStruct receiveObj = currentSock.recv();
                if(receiveObj.errorCode <= 0)
                {
                    LOG("Someone has left!");
                    set.removeSock(currentSock);
                    ignoreSockets = {currentSock, listenSock};
                    set.broadcast("Someone Has Left!\r\n", ignoreSockets);
                } else
                {
                    LOG("Someone wrote: " + receiveObj.data);
                    string msg = "Someone wrote: " + receiveObj.data;
                    ignoreSockets = {currentSock, listenSock};
                    set.broadcast(msg, ignoreSockets);
                }
            }
        }
    }
}