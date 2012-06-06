/* 
 * File:   PacketSIP.h
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 1:29 PM
 */

#ifndef PACKETSIP_H
#define	PACKETSIP_H

#include <unordered_map>
#include <stdio.h>
#include "SDP.h"

#include "Packet.h"

namespace PacketHandler { 

typedef std::unordered_map<std::string, std::list<unsigned int>* > HMap;
    
class PacketSIP : public Packet {
public:
    PacketSIP();
    PacketSIP(const PacketSIP& orig);
    virtual ~PacketSIP();
    
    char* getField(std::string header, unsigned int listPosition = 0);
    // sets the content to the field
    void setField(std::string header, char* content, unsigned int listPosition = 0);
    
    unsigned int getFieldPosition(char* header);
    // binds the header with position
    void setFieldAndPosition(unsigned int pos, std::string header, char* content);
   
    std::string getReceivedHost();
    std::string getVPort();
    std::string getViaPort();
    
    bool setVHost(std::string host);
    bool setVRport(std::string host);
    bool setVReceivedHost(std::string host);
    bool setRequestLineHost(std::string requestType,std::string host);   
    bool setMHost(std::string host);
    bool setToHost(std::string host);
    bool setViaHost(std::string host);
    bool setViaReceivedHost(std::string host);    
    bool setViaRport(std::string host);
    bool setContactHost(std::string host);
   
    bool checkMethod(std::string method);
    
    SDP* getSDP();
    void setSDP(SDP* sdp);
    
    std::string& getXML();
    
    std::string getMsg();
    // Request and response header fields
    
//    unsigned int AcceptContact;
//    unsigned int AllowEvent;
//    unsigned int CallID;
//    unsigned int Contact;
//    unsigned int ContentEncoding;
//    unsigned int ContentLenght;
//    unsigned int ContentType;
//    unsigned int Event;
//    char* From;
//    char* ReferTo;
//    char* ReferredBy;
//    char* RejectContact;
//    char* Subject;
//    char* To;
//    char* Via;
//    char* SDP;
private:
    char* contents_[100];
    HMap headers_;
    SDP* sdp_;
    std::string xmlBody_;
};

}

#endif	/* PACKETSIP_H */

