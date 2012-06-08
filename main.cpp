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

    if(chat) {
        std::string message = "";
        ConnectionConfiguration cconf = ApplicationManager::getInstance().loadConfigurationFromFile(filename);        
        cconf.outputToFile = false;
        Connection* conn = new Connection(cconf);
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
    } else {
        std::string fileToSendName="";
        ConnectionConfiguration cconf = ApplicationManager::getInstance().loadConfigurationFromFile(filename);        
        cconf.outputToFile = true;
        Connection* conn = new Connection(cconf);
        pthread_t connectionThread = conn->init();
        
        while (!(fileToSendName=="quit")) {        
            ifstream fileToSend;
            std::cout << "File to send (type path): " << std::endl;
            cin >> fileToSendName; 
            fileToSend.open(fileToSendName);
            if(fileToSend.is_open()) {
                while ( !fileToSend.eof() ) {
                    char s = fileToSend.get();
                    if(fileToSend.good())
                        conn->getOutputStream() << s;
                }
                conn->write();
                fileToSend.close();
            } else {
                std::cout << "No such file. Try again or type 'quit' to finish." << std::endl;
            }
        }
        std::cout << "Closing connection" << std::endl;
        pthread_join( connectionThread, NULL);
    }
    
    return 0;
}
