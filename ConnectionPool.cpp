/* 
 * File:   ConnectionPool.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 17, 2012, 12:55 PM
 */

#include "ConnectionPool.h"

int ConnectionPool::getPosition() {
        return numberOfElements;
}

void ConnectionPool::addConnection(Connection* connection, int pos = numberOfElements) {
    if(numberOfElements < MAX_CONN) {
        connections[pos] = connection;
    }
}

Connection* ConnectionPool::getConnection(std::string from) {
        for(int i=0; i<MAX_CONN; i++) {
                if(!(connections.is_null(i))) {
                        if(connections[i].getClientIP() == from)
                                return &connections[i];
                }
        }
        return NULL;
}

int ConnectionPool::count() {
        int counter = 0;
        for(int i=0;i<MAX_CONN;i++)
                if(!(connections.is_null(i)))
                        counter++;
        return counter;
}

bool ConnectionPool::isFull() {
        return !(count() < MAX_CONN);
}
