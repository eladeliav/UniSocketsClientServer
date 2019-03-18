//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include "UniSocket.h"
#include <thread>

using std::string;
using std::thread;

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
            int sendResult = 0;
            sendResult = sock.send(userInput);
            if (sendResult <= -1)
                connected = false;
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

    bool connected = true;
    do
    {
        UniSocketStruct receiveStatus = client.recv();
        if (receiveStatus.errorCode > 0)
        {
            std::cout << receiveStatus.data << std::endl;
            std::cout << ">";
        }
        else
            connected = false;
    } while (connected);

    return 0;
}

