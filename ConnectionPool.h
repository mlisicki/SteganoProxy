/* 
 * File:   ConnectionPool.h
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:55 PM
 */

#ifndef CONNECTIONPOOL_H
#define	CONNECTIONPOOL_H

#include <iostream>
#include <string>

#include "Connection.h"

const int MAX_CONN = 6;

/*
 * Connection container
 */
class ConnectionPool {
public:
        Connection* connections[MAX_CONN];
private:

    int numberOfElements;

    ConnectionPool() {
        numberOfElements = 0;
    }
    
    ConnectionPool(const ConnectionPool&);
    ~ConnectionPool();

public:
    static ConnectionPool& getInstance() {
            static ConnectionPool instance;
            return instance;
    }

    // Returns first free position
    int getPosition();

    void addConnection(Connection* connection, int pos);

    Connection* getConnection(std::string from);

    int count();

    bool isFull();
    
}; //class ConnectionPool

#endif /* NETWORK_CONNECTIONPOOL_H_ */