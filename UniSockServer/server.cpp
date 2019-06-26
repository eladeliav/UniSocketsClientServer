//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include <sstream>
#include "UniSockets/UniSocket.hpp"
#include <array>
#include <cstring>
#include <vector>
#include <algorithm>
#include <thread>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room!\n"
#define SEND_FILE_COMMAND "SEND_FILE"
#define SENDING_ALERT "SENDING_FILE"

using std::string;
using std::array;
using std::vector;
void splitRequestAndParams(string commandAndParams, string &command, vector<string> &paramsVector)
{
    command = commandAndParams;
    if (commandAndParams.find(' ') != string::npos)
    {
        command = command.erase(commandAndParams.find(' '));

        string params = commandAndParams.substr(commandAndParams.find(' ') + 1);

        std::stringstream test(params);
        std::string segment;
        while (std::getline(test, segment, ' '))
        {
            paramsVector.push_back(segment);
        }
    }
}

template<class T>
void Vec_RemoveAll(vector<T>& vec, T val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

void handleClient(UniSocket client, vector<UniSocket>& allClients, bool& running)
{
    char buf[DEFAULT_BUFFER_LEN];
    while(running)
    {
        memset(buf, '\0', DEFAULT_BUFFER_LEN);
        try
        {
            client.recv(buf);
        }
        catch (UniSocketException &e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT)
            {
                LOG(e);
                LOG("Someone has left!");
                Vec_RemoveAll(allClients, client);
                try
                {
                    UniSocket::broadcastToSet("Someone Has Left!", allClients, false);
                } catch (UniSocketException &e)
                {
                    LOG(e);
                }
                break;
            }
        }

        std::string msg = buf;
        if(msg.empty())
            continue;
        LOG("Someone wrote: " << msg);
        msg = "Someone wrote: " + msg;
        try
        {
            UniSocket::broadcastToSet(msg, allClients, false, client);
        } catch (UniSocketException &e)
        {
            LOG(e);
            LOG("Someone has left!");
            Vec_RemoveAll(allClients, client);
            try
            {
                UniSocket::broadcastToSet("Someone Has Left!", allClients, false);
            } catch (UniSocketException &e)
            {
                LOG(e);
            }
            continue;
        }
    }
}

int main()
{
    UniSocket listenSock(DEFAULT_PORT, SOMAXCONN);
    //UniSocketSet set(listenSock);
    vector<UniSocket> allClients;
    bool running = true;

    while (running)
    {
        UniSocket newClient = UniSocket();
        try
        {
            newClient = listenSock.acceptIntervals();
        }
        catch(UniSocketException& e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT)
            {
                LOG(e);
                break;
            }
        }

        if(newClient.valid())
        {
            newClient.send(WELCOME_MSG, sizeof(WELCOME_MSG));
            LOG("Someone Has Joined!");
            UniSocket::broadcastToSet("Someone Has Joined!", allClients, false);
            allClients.push_back(newClient);
            std::thread newClnThread = std::thread(handleClient, newClient, std::ref(allClients), std::ref(running));
            newClnThread.detach();
        }
    }
    return 0;
}