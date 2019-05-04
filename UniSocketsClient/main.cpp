//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include "UniSocket.h"
#include <thread>

using std::string;
using std::thread;

#define LOG(x) std::cout << x << std::endl

void sendMessages(UniSocket& sock)
{
    static string userInput;
    static bool connected = true;
    do
    {
        std::cout << "> ";
        std::cin >> std::ws;
        std::getline(std::cin, userInput, '\n');
        if (userInput.size() > 0)
        {
            try
            {
                sock.send(userInput.c_str());
            }catch(UniSocketException& e)
            {
                connected = false;
            }
        }
    } while (connected);
}

int main()
{
    UniSocket client;
    try
    {
        client = UniSocket("127.0.0.1", 5400);
    }catch(UniSocketException& e)
    {
        std::cout << e << std::endl;
        return 1;
    }



    if(!client.valid())
        return 1;

    std::thread sendMessagesThread(sendMessages, std::ref(client));
    sendMessagesThread.detach();
    const char* buf = nullptr;
    bool connected = true;
    do
    {
        try
        {
            buf = client.recv();
        }catch(UniSocketException& e)
        {
            connected = false;
            continue;
        }
        std::cout << buf << std::endl;
        std::cout << ">";
    } while (connected);
    delete buf;
    return 0;
}

