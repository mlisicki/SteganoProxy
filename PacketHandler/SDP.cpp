/* 
 * File:   SDP.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 21, 2012, 4:40 AM
 */

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include "../ApplicationManager.h"

#include "SDP.h"

using namespace PacketHandler;

SDP::SDP() {
    contents_[0] = NULL;
}

SDP::SDP(const SDP& orig) {
    
}

SDP::~SDP() {
}

char* SDP::getField(std::string header, unsigned int listPosition) {
    std::list<unsigned int>::iterator it;
    if(headers_.empty())
        return NULL;
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
}

void SDP::setField(std::string header, char* content,unsigned int listPosition) {
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

unsigned int SDP::getFieldPosition(char* header) {

}

void SDP::setFieldAndPosition(unsigned int pos, std::string header, char* content) {    
    if(header!="") {
        if(headers_.find(header)==headers_.end()) {
            std::list<unsigned int>* newList = new std::list<unsigned int>();
            headers_.insert(SDPHMap::value_type(header,newList));
        }
        headers_[header]->push_back(pos);
        contents_[pos] = content;
        contents_[pos+1] = NULL;
    }
}

int SDP::getRTPPort() {
    char* ch;  
    if((ch = getField("m"))==NULL)
        return -1;
    
    std::string out;
    while(*ch!=' ') ch++;
    ch++;
    while(*ch!=' ') {
        out += *ch;
        ch++;
    }
    
    std::stringstream convertion(out);
    int port;
    convertion >> port;
    
    return port;
}

bool SDP::setRTPPort(int port) {
    char* ch;
    char portString[6]; 
    char* newField = new char[50];

    sprintf(portString, "%d", port);
    if((ch = getField("m"))==NULL)
        return false;
    int i=0; 
    while(*ch!=' ') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;
    while(*ch!=' ') ch++;
    int pS = 0;
    while(portString[pS]!='\0') {
        newField[i] = portString[pS];
        i++;
        pS++;
    }
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

std::string SDP::getRTPIP() {
    std::string out = "";
    char* ch;  
    if((ch = getField("c"))==NULL)
        return out;
    
    while(*ch!=' ') ch++;
    ch++;
    while(*ch!=' ') ch++;
    ch++;    
    while(*ch!='\r' && *ch!='\n') {
        out += *ch;
        ch++;
    }
   
    return out;
}

bool SDP::setRTPIP(std::string ipString) {
    char* ch;
    char* newField = new char[50];

    if((ch = getField("c"))==NULL)
        return false;
    int i=0; 
    while(*ch!=' ') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;
    while(*ch!=' ') {
        newField[i] = *ch;
        ch++;
        i++;
    }
    newField[i] = *ch;
    ch++; 
    i++;
    
    int iS = 0;
    while(iS < ipString.size()) {
        newField[i] = ipString[iS];
        iS++;
        i++;
    }
    newField[i] = '\r';
    newField[i+1] = '\n';
  
    setField("c", newField);
    return true;
}

std::string SDP::getMsg() {
    std::string msg = "";
    
    SDPHMap::const_iterator sortedElements[100];
    SDPHMap::const_iterator it;
    std::list<unsigned int>::const_iterator lit;
    for ( it=headers_.begin() ; it != headers_.end(); ++it ) {
        for ( lit=(it->second)->begin() ; lit != (it->second)->end(); ++lit ) {
            sortedElements[(*lit)] = it;
        }
    }

    int i = 0;
    while(contents_[i]!=NULL) {
        msg += sortedElements[i]->first;
        msg += "=";
        msg += ApplicationManager::getInstance().readLine(contents_[i]);
        msg += "\r\n";
        i++;
    }
    
    if(i!=0) msg += "\r\n";
    
    return msg;
}