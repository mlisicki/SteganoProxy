/* 
 * File:   main.cpp
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:33 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "PacketHandler/Packet.h"
#include "Connection.h"
#include "ApplicationManager.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    char* filename = NULL;
    bool chat=false;
    
    for(int i=0; i<argc; i++) {
        if(strcmp(argv[i],"-c")==0 && i+1<argc)
            filename = argv[i+1];
        else if(strcmp(argv[i],"--chat")==0)
            chat=true;
    }
    
    if(argc<=2 || filename==NULL) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprintf(stderr, "\nUsage: steganoproxy -c configuration_file.xml [--chat]\n");
        exit(EXIT_FAILURE);
    }
    
    string message = "";
    ConnectionConfiguration cconf = ApplicationManager::getInstance().loadConfigurationFromFile(filename);        
    Connection* conn = new Connection(cconf);
    pthread_t connectionThread = conn->init();

//    if(chat) {
        std::cout << "Type a message to start chatting" << std::endl;

        while (!(message=="quit")) {
            std::cout << "> ";
            cin >> message;
            conn->getOutputStream() << message;
            conn->write();
            std::cout << std::endl;
//        }
//    } else {
//        ifstream fileToSend;
//        std::string fileToSendName;
//        std::cout << "File to send (type path): " << std::endl;
//        cin >> fileToSendName; 
//        fileToSend.open(fileToSendName);
//        fileToSend >> conn->getOutputStream();
//        conn->write();
//        fileToSend.close();
    }
    
    std::cout << "Closing connection" << std::endl;
    pthread_join( connectionThread, NULL);
    
    return 0;
}
