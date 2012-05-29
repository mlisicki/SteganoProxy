/* 
 * File:   Packet.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 17, 2012, 12:53 PM
 */

#include "Packet.h"

using namespace PacketHandler;

// exceptionally add functions inside namespace to be able to overload friend
// class operators
namespace PacketHandler {
    
std::istream& operator>>(std::istream &is, Packet &obj) {
    is >> obj.msg_;
    return is;
}

std::ostream& operator<<(std::ostream &os, const Packet &obj) {
    os << obj.msg_;
    return os;
}

}