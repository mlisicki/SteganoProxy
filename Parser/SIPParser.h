/* 
 * File:   SIPParser.h
 * Author: Michal Lisicki
 *
 * Created on January 21, 2012, 12:56 AM
 */

#ifndef SIPPARSER_H
#define	SIPPARSER_H

#include "../PacketHandler/PacketSIP.h"

#include "Symbol.h"

namespace Parser {

class SIPParser {
public:
    SIPParser();
    SIPParser(const SIPParser& orig);
    virtual ~SIPParser();
    
    PacketHandler::PacketSIP generatePacket(char* msg);
    
    void next();
    int accept(SymbolValue atom);
    std::string ident();
    std::string name();
    std::string s();    
//    void eq();
    std::string content();        
    std::string header(); 
    std::string SDPheader();    
    std::string method();    
    void document();
private:
    Symbol symbol_;
    Symbol back_;
    char* scanStream_;
};

}

#endif	/* SIPPARSER_H */
