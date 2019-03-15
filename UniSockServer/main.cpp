//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include "UniSocket.h"
#include <vector>
#include <thread>

using std::string;
using std::vector;
using std::thread;

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room"

void handleClient(UniSocket& sock, vector<UniSocket> set);

int main()
{
    int port = DEFAULT_PORT;
    bool running = false;
    char buf[DEFAULT_BUFFER_LEN];
    int receivedBytes = 0;
    string receivedString;

    LOG("Slightly Less Shitty Chat");
//    LOG("Enter port: ");
//    std::cin >> port;
//
//    while(port < 0 || port > 65535)
//    {
//        LOG("Invalid Port. Must be between 0 and 65535");
//        std::cin >> port;
//    }

    UniSocket serverSocket(port, SOMAXCONN);

    LOG("listening on " << port);

    running = true;

    vector<UniSocket> allClients;
    
    do
    {
        UniSocket newClient = serverSocket.accept();
        LOG("New Client connected: " << newClient.getIp());
        newClient.send("Welcome");
        allClients.push_back(newClient);
    } while(running);



}

void handleClient(UniSocket& sock, vector<UniSocket> set)
{
    while(true)
    {
        string receivedString = sock.recv();
        if(receivedString.empty())
        {
            sock.close();
            break;
        }
        LOG(sock.getIp() << ": " << receivedString);
        if(receivedString == "EXIT")
        {
            sock.close();
            break;
        }

        for(auto& value: set)
        {
            if(value != sock)
            {
                value.send(receivedString);
            }
        }

    }
}