/* 
 * File:   SDP.h
 * Author: Michal Lisicki
 *
 * Created on January 21, 2012, 4:40 AM
 */

#ifndef SDP_H
#define	SDP_H

#include <list>
#include <iostream>
#include <string>
#include <unordered_map>

namespace PacketHandler { 

typedef std::unordered_map<std::string, std::list<unsigned int>* > SDPHMap;

class SDP {
public:
    SDP();
    SDP(const SDP& orig);
    virtual ~SDP();
    
    // Each header have a list of elements it can access. Each element is added
    // whenever content with identical header appears.
    // The method allows access to the specific field of that list.
    char* getField(std::string header, unsigned int listPosition = 0);
    // sets the content to the field
    void setField(std::string header, char* content, unsigned int listPosition = 0);
    
    unsigned int getFieldPosition(char* header);
    // binds the header with position
    void setFieldAndPosition(unsigned int pos, std::string header, char* content);    
    
    int getRTPPort();
    bool setRTPPort(int port);
    std::string getRTPIP();    
    bool setRTPIP(std::string ipString);
    
    std::string getMsg();
//    std::string Version;
//    std::string Owner;
//    std::string SessionName;
//    std::string SessionInformation;
//    std::string URI;
//    std::string Email;
//    std::string Phone;
//    std::string ConnectionInfo;
//    std::string BandwidthInfo;
//    std::string TimeSession;
//    std::string RepeatTimes;
//    std::string TZCorrections;
//    std::string EncryptionKey;
//    std::string AttributeLines;
//    std::string MediaInformation;
//    std::string MediaAttributes;  
private:
    char* contents_[100];
    SDPHMap headers_;
};

}

#endif	/* SDP_H */

