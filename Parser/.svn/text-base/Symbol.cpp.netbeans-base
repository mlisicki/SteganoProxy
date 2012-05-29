/* 
 * File:   Symbol.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 18, 2012, 1:20 PM
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
    if((c>='a' && c<='z') || (c>='A' && c<='Z'))  value_ = letter;
    else if(c>='0' && c<='9')                     value_ = number;
    else if(c=='=')                               value_ = equal;
    else if(c=='\n')                              value_ = nextline;
    else if(c=='\0')                              value_ = nullchar;
    else if(c==0x20 || c==0x09 || 
            c==0x0b || c==0x0c || 
            c=='\r')                              value_ = white;
    else if((c>=0x21 && c<=0x3c) ||               // exclude '='
            (c>=0x3e && c<=0x40) ||               // exclude A-Z
            (c>=0x5b && c<=0x60) ||               // exclude a-z
            (c>=0x7b && c<=0x7e))                 // exclude DEL
                                                  value_ = sym;  // e.g !@#$%^&*() and similar signs
    else                                          return 0;
    return 1;
}

bool Symbol::operator==(SymbolValue other) const {
    return (value_ == other);
}