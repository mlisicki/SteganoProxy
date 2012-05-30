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
#include "ApplicationManager.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    if(argc!=2) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprintf(stderr, "\nUsage: steganoproxy configuration_file.xml\n");
        exit(EXIT_FAILURE);
    }
    
    string message = "";
    ConnectionConfiguration cconf = ApplicationManager::getInstance().loadConfigurationFromFile(argv[1]);
    Connection* conn = new Connection(cconf);
    conn->init();
    std::cout << "Type a message to start chatting" << std::endl;
    
    while (!(message=="quit")) {
        std::cout << "> ";
        cin >> message;
        conn->getOutputStream() << message;
        conn->write();
        std::cout << std::endl;
    }
    return 0;
}
