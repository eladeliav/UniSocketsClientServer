//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include "UniSocket.h"

using std::string;

int main()
{
    try
    {
        UniSocket client("127.0.0.1", 5400);
        string sendString;

        do
        {
            std::cout << "The Server sent back: " << client.recv() << std::endl;
            std::cout << ">";
            std::cin >> sendString;
            client.send(sendString);
        }while(sendString != "EXIT");
        client.close();
    }catch(UniSocketException e)
    {
        std::cout << e << std::endl;
    }


    return 0;
}

