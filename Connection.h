/* 
 * File:   Connection.h
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:55 PM
 */

#ifndef CONNECTION_H
#define	CONNECTION_H

#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
        #include <windows.h>
        #include <sys/types.h>
        #include <winsock.h>
        #include <unistd.h> // usleep function
        typedef int socklen_t;
#else
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <unistd.h>
        #include <netinet/in.h>
        #include <netdb.h>
        #include <arpa/inet.h>
#endif

#include "PacketHandler/Packet.h"
#include "ConnectionConfiguration.h"
        
#include "Connection.h"

class Connection {
public:
    Connection(ConnectionConfiguration& cconf);
    Connection(const Connection& orig);
    virtual ~Connection();

    bool init();
    
    std::istringstream& getInputStream();
    std::ostringstream& getOutputStream();

    // Sends the stream content
    void write();

    // Reads the stream content
    void read();

    // Closes connection
    void shutdown();

    int getPort();
    void setPort(int port);

    bool isClosed();
    bool isConnected();
private:
    std::string sipProxyHost_;
    std::string rtpProxyHost_;
    std::string localHost_;

    int sipExternalPort_;
    int sipInternalPort_;
    int sipProxyPort_;
    int sipApplicationPort_;

    int rtpExternalPort_;
    int rtpInternalPort_;
    int rtpProxyPort_;
    int rtpApplicationPort_;    

    std::istringstream sin_;
    std::ostringstream sout_;

    int rtpMean_;
    int rtpStdDev_;
    
    bool connected_;
    bool dataOutReady_;
    
    unsigned int SIPApplicationSock_, RTPApplicationSock_, SIPProxySock_, RTPProxySock_;

    // For sending
    struct sockaddr_in siSIPProxy_, siSIPApplication_, siRTPProxy_, siRTPApplication_;
    // For receiving
    struct sockaddr_in siSIPApplicationListen_, siRTPApplicationListen_, siSIPProxyListen_, siRTPProxyListen_;

//    pthread_mutex_t writeMessageMutex_ = PTHREAD_MUTEX_INITIALIZER;    
    
    void wait(float seconds);

    PacketHandler::Packet receivePacket();
    int sendPacket(PacketHandler::Packet packet);
    
    static void *listenOnSockets( void *ptr );
    
};

#endif	/* CONNECTION_H */
