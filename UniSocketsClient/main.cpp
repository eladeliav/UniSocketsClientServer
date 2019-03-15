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
            sock.send(userInput, sendResult);
            if (sendResult <= -1)
                connected = false;
        }
    } while (connected);
}

int main()
{
    UniSocket client("127.0.0.1", 5400);

    if(!client.valid())
        return 1;

    std::thread sendMessagesThread(sendMessages, std::ref(client));
    sendMessagesThread.detach();

    string receivedString;
    bool connected = true;
    do
    {
        int bytesReceived = 0;
        receivedString = client.recv(bytesReceived);
        if (bytesReceived > 0)
        {
            std::cout << receivedString;
            std::cout << ">";
        }
        else
            connected = false;
    } while (connected);

    return 0;
}

