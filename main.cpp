/* 
 * File:   main.cpp
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:33 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include "PacketHandler/Packet.h"
#include "Connection.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    string message = "";
    Connection* conn = new Connection();
    pthread_t connectionThread = conn->init();
    std::cout << "Type a message to start chatting" << std::endl;
    
    while (!(message=="quit")) {
        std::cout << "> ";
        cin >> message;
        conn->getOutputStream() << message;
        conn->write();
        std::cout << std::endl;
    }
    
    std::cout << "Closing connection" << std::endl;
    pthread_join( connectionThread, NULL);
    
    return 0;
}
