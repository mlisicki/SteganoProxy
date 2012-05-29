/* 
 * File:   Packet.h
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:53 PM
 */
#ifndef PACKET_H_
#define PACKET_H_

#ifdef _WIN32
        typedef int socklen_t;
#endif

#ifdef _WIN32
        #include <windows.h>
        #include <sys/types.h>
        #include <winsock.h>
        #include <unistd.h> // usleep function
        typedef int socklen_t;
#else
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <arpa/inet.h>
#endif

#include <iostream>

namespace PacketHandler {       
        
/*
 * General structure of incoming packet
 */
class Packet {
protected:
    unsigned char msgNo_;
    socklen_t length_;
    std::string source_;
    std::string dest_;
    char msg_[8048];
public:
    Packet() : msgNo_(0), length_(0) {}
//    Packet(unsigned char type, std::string msg) : type_(type), msgNo_(0), length_(0), msg_(msg) {}
    ~Packet() {}

    friend std::istream& operator>>(std::istream &is, Packet &obj);
    friend std::ostream& operator<<(std::ostream &os,const Packet &obj);
};

}

#endif /* PACKET_H_ */