/* 
 * File:   PacketRTP.h
 * Author: Michal Lisicki
 *
 * Created on April 8, 2012, 4:22 AM
 */

#ifndef PACKETRTP_H
#define	PACKETRTP_H

#include <unordered_map>
#include <string>
#include "Packet.h"

namespace PacketHandler { 

typedef std::unordered_map<std::string, std::pair<int, int>* > HPairMap;

class PacketRTP : public Packet {
public:
    PacketRTP(char* msg, int packetSize);
    PacketRTP(const PacketRTP& orig);
    virtual ~PacketRTP();
    
    unsigned char getField(std::string header);
    int getSequenceNumber();
    void setSequenceNumber(int seqNum);
    std::string getPayload();
    std::string getMsg();
private:
    HPairMap headers_;
    int msgSize_;
};

}

#endif	/* PACKETRTP_H */

