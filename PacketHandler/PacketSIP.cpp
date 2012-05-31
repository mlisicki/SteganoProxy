/* 
 * File:   PacketSIP.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 17, 2012, 1:29 PM
 */

#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../ApplicationManager.h"

#include "PacketSIP.h"

using namespace PacketHandler;

PacketSIP::PacketSIP() {
    sdp_ = new SDP();
    contents_[0] = NULL;
    xmlBody_ = "";
}

PacketSIP::PacketSIP(const PacketSIP& orig) {
}

PacketSIP::~PacketSIP() {
    delete sdp_;
}

char* PacketSIP::getField(std::string header, unsigned int listPosition) {
    std::list<unsigned int>::iterator it;
    if(headers_.find(header)==headers_.end())
        return NULL;
    try {
        it = headers_[header]->begin();
    } catch (std::out_of_range& oor) {
        return NULL;
    }
    if(it==headers_[header]->end())
        return NULL;
    for(int i=0; i<listPosition;i++) {
        it++;
        if(it==headers_[header]->end())
            return NULL;
    }

    return contents_[*it];
    
//  unsigned int pos;    
//  try {
//       pos = headers_[header];
//  } catch (std::out_of_range& oor) {
//       return NULL;
//  }
//
//  return contents_[pos];
}

void PacketSIP::setField(std::string header, char* content, unsigned int listPosition) {
    if(header!="") {
        std::list<unsigned int>::iterator it;
        if(headers_.empty())
            return;
        if(headers_.find(header)==headers_.end()) {
            return;
        }
        try {
            it = headers_[header]->begin();
        } catch (std::out_of_range& oor) {
            return;
        }
        if(it==headers_[header]->end())
            return;        
        for(int i=0; i<listPosition;i++) {
            it++;
            if(it==headers_[header]->end())
                return;
        }
        contents_[*it] = content;
    }
}

unsigned int PacketSIP::getFieldPosition(char* header) {

}

void PacketSIP::setFieldAndPosition(unsigned int pos, std::string header, char* content) {
//    char* ch = content;
//    std::cout << "[" << pos << "] header='" << header;
//    std::cout << "' \t\tcontent='";
//    while(*ch!='\n' && *ch!='\r' && *ch!='\0') {
//        std::cout << *ch;
//        ch++;
//    }
//    std::cout << "'";
//    std::cout << std::endl;
//    
    
    if(header!="") {
        if(headers_.find(header)==headers_.end()) {
            std::list<unsigned int>* newList = new std::list<unsigned int>();
            headers_.insert(HMap::value_type(header,newList));
        }
        headers_[header]->push_back(pos);
        contents_[pos] = content;
        contents_[pos+1] = NULL;
    }
    
//    if(header!="") {
//        headers_.insert(HMap::value_type(header,pos));
//        contents_[pos] = content;
//        contents_[pos+1] = NULL;
//    }
}

std::string PacketSIP::getReceivedHost() {
    char* ch;  
    std::string out = "";
    
    if((ch = getField("Via"))==NULL)
        return out;
    if((ch = strstr(ch,"received"))==NULL)
        return out;
        
    while(*ch!='=') ch++;
    ch++;
    while(*ch!=' ' && *ch!='\r' && *ch!='\n') {
        out += *ch;
        ch++;
    }
    
    return out;
}

bool PacketSIP::setVHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[300];

    if((ch = getField("v"))==NULL)
        return false;
    if((found = strstr(ch,"SIP/2.0/UDP"))==NULL)
        return false;
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!=' ') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("v", newField);
    
    return true;
}

bool PacketSIP::setVReceivedHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[300];

    if((ch = getField("v"))==NULL)
        return false;
    if((found = strstr(ch,"received"))==NULL)
        return false;
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!='=') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';' && *ch!='\r' && *ch!='\n')
        ch++;
    
    if(*ch==';') {  
        newField[i] = *ch;
        ch++; 
        i++;

        while(*ch!='\r' && *ch!='\n') {
            newField[i] = *ch;
            ch++;
            i++;
        }
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("v", newField);
    
    return true;
}

bool PacketSIP::setVRport(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[300];

    if((ch = getField("v"))==NULL)
        return false;
    if((found = strstr(ch,"rport"))==NULL)
        return false;
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!='=') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("v", newField);
    
    return true;
}

bool PacketSIP::setViaHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[200];

    if((ch = getField("Via"))==NULL)
        return false;    
    if((found = strstr(ch,"SIP/2.0/UDP"))==NULL)
        return false;
       
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!=' ') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
      
    setField("Via", newField);
    
    return true;
}

bool PacketSIP::setViaReceivedHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[300];

    if((ch = getField("Via"))==NULL)
        return false;
    if((found = strstr(ch,"received"))==NULL)
        return false;
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!='=') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!='\r' && *ch!='\n')
        ch++;
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("Via", newField);
    
    return true;
}

bool PacketSIP::setViaRport(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[300];

    if((ch = getField("Via"))==NULL)
        return false;
    if((found = strstr(ch,"rport"))==NULL)
        return false;
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    while(*ch!='=') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("Via", newField);
    
    return true;
}

bool PacketSIP::setRequestLineHost(std::string requestType,std::string host) {
    char* ch;
    char* found;
    char* newField = new char[200];

    if((ch = getField(requestType))==NULL)
        return false;
    if((found = strstr(ch,"@"))==NULL)
        return false;

    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }

    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!=';' && (*ch==':' || *ch=='.' || (*ch>=48 && *ch <= 57)))
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField(requestType, newField);
    
    return true;
}

bool PacketSIP::setMHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[200];

    if((ch = getField("m"))==NULL)
        return false;
    if((found = strstr(ch,"@"))==NULL)
        return false;
        
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!='>' && *ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("m", newField);
    
    return true;
}

bool PacketSIP::setToHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[200];

    if((ch = getField("To"))==NULL)
        return false;
    if((found = strstr(ch,"@"))==NULL)
        return false;
        
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!='>')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("To", newField);
    
    return true;
}

bool PacketSIP::setContactHost(std::string host) {
    char* ch;
    char* found;
    char* newField = new char[200];

    if((ch = getField("Contact"))==NULL)
        return false;
    if((found = strstr(ch,"@"))==NULL)
        return false;
        
    int i=0;
    while(ch!=found) {
        newField[i] = *ch;
        ch++;
        i++;
    }
        
    newField[i] = *ch;
    ch++; 
    i++;

    int hS = 0;
    while(hS < host.size()) {
        newField[i] = host[hS];
        hS++;
        i++;
    }
    
    while(*ch!='>' && *ch!=';')
        ch++;
    
    newField[i] = *ch;
    ch++; 
    i++;
    
    while(*ch!='\r' && *ch!='\n') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("Contact", newField);
    
    return true;
}

bool PacketSIP::checkMethod(std::string method) {
    std::string out = "";
    char* ch;
    ch = getField("SIP/2.0");
    if(ch!= NULL) {
        while(*ch!='\r' && *ch!='\n') {
                out += *ch;
                ch++;
        }
    }
    return ((getField(method)!=NULL) || (out==method));
}

SDP* PacketSIP::getSDP() {
    return sdp_;
}

std::string& PacketSIP::getXML() {
    return xmlBody_;
}

void PacketSIP::setSDP(SDP* sdp) {
    sdp_ = sdp;
}

std::string PacketSIP::getMsg() {
    std::string msg = "";
    
    char* ct = getField("Content-Type");
    char* c = getField("c");
    
    if(!((ct!=NULL && (strstr(ct,"xml"))!=NULL) || 
       (c!=NULL && (strstr(c,"xml"))!=NULL))) {
            // Set Content-Length
            std::string sdp = sdp_->getMsg();
            char* contentLength = new char[10];
            snprintf(contentLength, sizeof(contentLength), "%d", sdp.length());
            int strLen = strlen(contentLength);
            contentLength[strLen] = '\r';
            contentLength[strLen+1] = '\n';
            setField("Content-Length",contentLength);
    }
    
    HMap::const_iterator sortedElements[100];
    HMap::const_iterator it;
    std::list<unsigned int>::const_iterator lit;
    for ( it=headers_.begin() ; it != headers_.end(); ++it ) {
        for ( lit=(it->second)->begin() ; lit != (it->second)->end(); ++lit ) {
            sortedElements[(*lit)] = it;
        }
    }

    int i = 0;
    while(contents_[i]!=NULL) {
//        std::cout << "===" << sortedElements[i]->first << "===" << ApplicationManager::getInstance().readLine(contents_[i]) << "===" << std::endl;
//        std::string first = sortedElements[i]->first;
        msg += sortedElements[i]->first;
        if(i!=0) msg += ":";
        msg += " ";
        msg += ApplicationManager::getInstance().readLine(contents_[i]);
        msg += "\r\n";
        i++;
    }
    
    msg += "\r\n";
    
    if((ct!=NULL && (strstr(ct,"xml"))!=NULL) || 
       (c!=NULL && (strstr(c,"xml"))!=NULL)) {
        msg += getXML();
        // msg.resize(msg.size()-2);
     //   msg += '\n';
    } else {
        msg += sdp_->getMsg();
    }
        
    return msg;
}
