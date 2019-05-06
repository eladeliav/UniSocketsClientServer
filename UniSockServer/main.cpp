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
#define WELCOME_MSG "Welcome to the chat room!\n"

using std::string;
using std::array;

int main()
{
    UniSocket listenSock(DEFAULT_PORT, SOMAXCONN);
    UniSocketSet set(listenSock);
    bool running = true;
    char buf[DEFAULT_BUFFER_LEN];

    while (running)
    {
        for (UniSocket &currentSock : set.getReadySockets())
        {
            memset(buf, '\0', DEFAULT_BUFFER_LEN);
            if (listenSock == currentSock)
            {
                UniSocket newClient;
                try
                {
                    newClient = listenSock.accept();
                }catch(UniSocketException& e)
                {
                    LOG(e);
                    continue;
                }
                set.addSock(newClient);
                newClient.send(WELCOME_MSG);
                LOG("Someone Has Joined!");
                set.broadcast("Someone Has Joined!", array<UniSocket, 2>{newClient, listenSock});
            } else
            {
                try
                {
                    currentSock.recv(buf);
                } catch (UniSocketException &e)
                {
                    LOG(e);
                    LOG("Someone has left!");
                    set.removeSock(currentSock);
                    try
                    {
                        set.broadcast("Someone Has Left!", array<UniSocket, 2>{currentSock, listenSock});
                    }catch(UniSocketException& e)
                    {
                        LOG(e);
                    }
                    continue;
                }

                LOG("Someone wrote: " << buf);
                string msg = "Someone wrote: " + string(buf);
                try
                {
                    set.broadcast(msg, array<UniSocket, 2>{currentSock, listenSock});
                }catch(UniSocketException& e)
                {
                    LOG(e);
                }
            }
        }
    }
    return 0;
}