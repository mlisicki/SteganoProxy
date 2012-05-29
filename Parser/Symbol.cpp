/* 
 * File:   Symbol.h
 * Author: mike
 * 
 * Created on December 6, 2011, 6:26 AM
 */

#include "Symbol.h"

SymbolValue Symbol::get() {
    return value_;
}

char Symbol::getCharacter() {
    return character_;
}

int Symbol::set(char c) {
    character_ = c;
    if((c>='a' && c<='z') || (c>='A' && c<='Z')) value_ = letter;
    else if(c>='0' && c<='9')                        value_ = number;
    else if(c=='<')                              value_ = opensym;
    else if(c=='>')                              value_ = closesym;
    else if(c=='/')                              value_ = slash;
    else if(c=='=')                              value_ = equal;
    else if(c=='"')                              value_ = qMark;        
    else if(c==0x20 || c==0x09 || 
            c==0x0a || c==0x0b ||
            c==0x0c || c==0x0d)                  value_ = white;
    else if((c>=0x21 && c<=0x2e) ||
            (c>=0x3a && c<=0x40) ||
            (c>=0x5b && c<=0x60) ||
            (c>=0x7b && c<=0x7e))                 value_ = sym;  // e.g !@#$%^&*() and similar signs
    else                                         return 0;
    return 1;
}

bool Symbol::operator==(SymbolValue other) const {
    return (value_ == other);
}