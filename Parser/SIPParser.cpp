/* 
 * File:   SIPParser.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 21, 2012, 12:56 AM
 */

#include <iostream>
#include <string>

#include "SIPParser.h"

using namespace XMLParser;

SIPParser::SIPParser() {
}

SIPParser::SIPParser(const SIPParser& orig) {
}

SIPParser::~SIPParser() {
}

PacketHandler::PacketSIP SIPParser::generatePacket(char* msg) {
    std::string hdr;  
    std::string sdpHdr;     
    scanStream_ = msg;
    PacketHandler::PacketSIP pktSIP;
    unsigned int pos = 0;
    
    // set the first character
    if(!symbol_.set(*scanStream_)) throw "Unexpected symbol in SIP";
 //   std::cout << scanStream_ << std::endl;
    
    while(!(symbol_==nextline || symbol_==nullchar || symbol_==white)) {
        // check if we're on the beginning of SIP message
        if(scanStream_ == msg) {
            hdr = method();
        } else {
            hdr = header();
        }
        pktSIP.setFieldAndPosition(pos,hdr,scanStream_);
        content();
        accept(nextline);
        pos++;
    }
    while(symbol_==nextline || symbol_==white) {
        s();
        accept(nextline);        
    }
    if(!(symbol_==nullchar)) {
        pos = 0;
        while(!(symbol_==nextline || symbol_==nullchar || symbol_==white)) {
            sdpHdr = SDPheader();
            pktSIP.getSDP()->setFieldAndPosition(pos,sdpHdr,scanStream_);
            content();
            accept(nextline);
            pos++;     
        }
    }
    
    return pktSIP;
}

/* ===Production rules=== */

// get the next symbol
void SIPParser::next() {
    scanStream_ = scanStream_++;
    if(!symbol_.set(*scanStream_)) {
        std::cout << "Error: '" << symbol_.getCharacter() << "'" << std::endl;
        
        throw "Unexpected symbol in SIP";
    }
}

int SIPParser::accept(SymbolValue atom) {
    if(symbol_ == atom) {
        next();
        return 1;
    } else { 
        throw "SIP Syntax error";
    }
    return 0;
}

std::string SIPParser::ident() {
    std::string identString;
    do {
        identString += symbol_.getCharacter();
        if(symbol_==number) accept(number);
        else if(symbol_==letter) accept(letter);
        else if(symbol_==sym) accept(sym);
        else break;
    } while(true);
    identString.erase(identString.end()-1);
    return identString;
}

std::string SIPParser::s() {
    std::string w = "";
    while(symbol_ == white) {
        w += symbol_.getCharacter();
        accept(white);
    }
    return w;
}

std::string SIPParser::content() {
    std::string cont = "";
    while(!(symbol_==nextline)) {
        cont += ident();
        while(symbol_==equal) {
            accept(equal);
            cont += symbol_.getCharacter();
            cont += ident();
        }
        cont += s();
    }
    return cont;
}

std::string SIPParser::header() {
    std::string name = ident();
    if(name!="" && name.at(name.size()-1)==':') {
        name.erase(name.size()-1,1);
    }
    s();
    return name;
}

std::string SIPParser::SDPheader() {
    std::string name = ident();
    if(symbol_==equal)
        accept(equal);
    s();
    return name;
}

std::string SIPParser::method() {
    std::string name = ident();
    s();
    return name;
}

void SIPParser::document() {
    method();
    while(!(symbol_==nullchar)) {
        header();
        content();
        accept(nextline);
    }
}
