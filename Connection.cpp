/* 
 * File:   Connection.cpp
 * Author: Michal Lisicki
 * 
 * Created on January 17, 2012, 12:55 PM
 */

#include <stdio.h> 
#include <cstring> 
#include <stdlib.h>
#include <pthread.h>
#include <algorithm>

#include "PacketHandler/PacketSIP.h"
#include "Parser/SIPParser.h"
#include "PacketHandler/PacketRTP.h"
#include "ApplicationManager.h"

#include "Connection.h"

#define BUFSIZE 8192

// const char* Connection::SIP_PROXY_HOST = "sip.tpad.com";
// const char* Connection::SIP_PROXY_HOST = "213.40.29.11";
const char* Connection::SIP_PROXY_HOST = "194.29.169.4";

Connection::Connection() {
    sipApplicationPort_ = 0;
    sipExternalPort_ = SIP_EXTERNAL_PORT;
    sipProxyPort_ = SIP_PROXY_PORT;    
    sipInternalPort_ = SIP_INTERNAL_PORT;
    sipProxyHost_.assign(SIP_PROXY_HOST);
    rtpExternalPort_ = RTP_EXTERNAL_PORT;
    rtpInternalPort_ = RTP_INTERNAL_PORT;
    rtpApplicationPort_ = 0; 
    rtpProxyPort_ = 0;
    rtpProxyHost_ = "";
    localHost_ = "192.168.1.11:5060"; // "192.168.1.13:5060";
    
    dataOutReady_ = false;
}

void* Connection::listenOnSockets( void *ptr ) {
    Connection* classPtr = (Connection*) ptr;
    int slen;
    int pktSize;
    Parser::SIPParser sipParser;
    char* content;
    std::string msgProxy = "";
    std::string msgApplication = "";
    std::string msgProxyRTP = "";
    std::string msgApplicationRTP = "";
    
    sockaddr_in siFrom;
    slen = sizeof(siFrom);
    char buf[BUFSIZE];

    std::string msg;
    int error;
 
    fd_set readSet;

    int fds[] = {classPtr->RTPProxySock_,classPtr->RTPApplicationSock_,
                 classPtr->SIPProxySock_,classPtr->SIPApplicationSock_};
    int fdMax = *std::max_element(fds,fds+4);
        
    while( 1 ) {
        FD_ZERO(&readSet);
        FD_SET(classPtr->RTPProxySock_,&readSet);    
        FD_SET(classPtr->RTPApplicationSock_,&readSet); 
        FD_SET(classPtr->SIPProxySock_,&readSet);    
        FD_SET(classPtr->SIPApplicationSock_,&readSet); 
        
        int retval = select(fdMax+1,&readSet,0,0,NULL);

        if(retval < 0) printf("select error\n");
        if(retval>0) {
            if(FD_ISSET(classPtr->RTPProxySock_,&readSet)) {
                if((pktSize = recvfrom(classPtr->RTPProxySock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                buf[pktSize] = '\0';
                printf("\nReceived RTP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));
                PacketHandler::PacketRTP pktRTP(buf, pktSize);
                // std::cout << (int)pktRTP.getSequenceNumber() << std::endl;
                // std::cout << pktRTP.getPayload() << std::endl;
                
                // prepare the message to send
                slen = sizeof(classPtr->siRTPApplication_);
                if (sendto(classPtr->RTPApplicationSock_, buf, pktSize,0, (struct sockaddr *) &(classPtr->siRTPApplication_),slen) == -1) {
                    perror("sending rtp to application failed");
                    close(classPtr->RTPApplicationSock_);
                }
            }
            if(FD_ISSET(classPtr->RTPApplicationSock_,&readSet)) {
                if((pktSize = recvfrom(classPtr->RTPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                buf[pktSize] = '\0';
                printf("\nReceived RTP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));
                
                // std::cout << buf << std::endl;
                PacketHandler::PacketRTP pktRTP(buf,pktSize);
                
                msgProxyRTP = pktRTP.getMsg();
                
                if (classPtr->dataOutReady_) {
                    pktRTP.setSequenceNumber(0);
                    classPtr->dataOutReady_ = false;
                    classPtr->sout_.str("");
                }
                                
                std::cout << msgProxyRTP << std::endl;
                
                // prepare the message to send
                slen = sizeof(classPtr->siRTPProxy_);    
                if (sendto(classPtr->RTPProxySock_, buf, pktSize,0, (struct sockaddr *) &(classPtr->siRTPProxy_),slen) == -1) {
                    perror("sending packet to sip proxy failed");
                    close(classPtr->RTPProxySock_);
                }
                
                msgProxyRTP.clear();
            }
            if(FD_ISSET(classPtr->SIPProxySock_,&readSet)) {
                if((pktSize = recvfrom(classPtr->SIPProxySock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }

                buf[pktSize] = '\0';
              //  printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);

                msgApplication = pktSIP.getMsg();                
                
                // prepare the message to send
                slen = sizeof(classPtr->siSIPApplication_);
                if (sendto(classPtr->SIPApplicationSock_, msgApplication.c_str(), msgApplication.size(),0, (struct sockaddr *) &(classPtr->siSIPApplication_),slen) == -1) {
                    perror("sending sip to application failed");
                    close(classPtr->SIPApplicationSock_);
                }
                msgApplication.clear();
            
            }
            if(FD_ISSET(classPtr->SIPApplicationSock_,&readSet)) {
                if((pktSize = recvfrom(classPtr->SIPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                buf[pktSize] = '\0';
              //  printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));       
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);

                msgProxy = pktSIP.getMsg();                
                
                // prepare the message to send
                slen = sizeof(classPtr->siSIPProxy_);    
                if (sendto(classPtr->SIPProxySock_, msgProxy.c_str(), msgProxy.size(),0, (struct sockaddr *) &(classPtr->siSIPProxy_),slen) == -1) {
                    perror("sending packet to sip proxy failed");
                    close(classPtr->SIPProxySock_);
                }
                msgProxy.clear();                
            }
        }
    }
}

bool Connection::init() {
#ifdef _WIN32
    WSADATA WsaData;
    WSAStartup(MAKEWORD(2,2), &WsaData);
#endif
    struct hostent *hp, *gethostbyname();
    pthread_t listeningThread;
    int slen;
    int pktSize;
    Parser::SIPParser sipParser;
    // Message for proxy (not from)
    std::string msgProxy = "";
    // Message for application
    std::string msgApplication = "";
    sockaddr_in siFrom;
    slen = sizeof(siFrom);
    char buf[BUFSIZE];
    
    int reuse_addr = 1;
    SIPApplicationSock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SIPApplicationSock_!=-1) {
            connected_ = true;
    }
  //  setsockopt(SIPApplicationSock_, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    
    SIPProxySock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SIPProxySock_!=-1) {
            connected_ = true;
    }
    
    struct addrinfo hints, *addrInfoResult;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    // converts human-readable text strings representing hostnames or IP 
    // addresses into a dynamically allocated linked list of struct addrinfo 
    // structure (instead of gethostname)
    if (getaddrinfo(sipProxyHost_.c_str(), NULL, NULL, &addrInfoResult) != 0) {   
        fprintf(stderr, "getaddrinfo() failed\n");
        exit(1);
    }

    std::cout << "Configuring SIP Communication... " << std::endl;    
    
    // Setting SIP proxy communication
    siSIPProxyListen_.sin_family = AF_INET;
    siSIPProxyListen_.sin_port = htons(sipExternalPort_);
    siSIPProxyListen_.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(SIPProxySock_,(struct sockaddr *)&siSIPProxyListen_, sizeof siSIPProxyListen_) == -1) {
        perror("binding datagram socket");
        exit(1);
    }

    siSIPProxy_.sin_family = AF_INET;
    siSIPProxy_.sin_port = htons(sipProxyPort_);
//    siSIPProxy_.sin_addr = ((struct sockaddr_in *) addrInfoResult->ai_addr)->sin_addr;       
    if (inet_aton(SIP_PROXY_HOST, (struct in_addr *)&(siSIPProxy_.sin_addr))==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    // Setting SIP application communication
    siSIPApplicationListen_.sin_family = AF_INET;
    siSIPApplicationListen_.sin_port = htons(sipInternalPort_);
//    siSIPApplicationListen_.sin_addr.s_addr = htonl(INADDR_ANY);
    if (inet_aton("127.0.0.1", (struct in_addr *)&(siSIPApplicationListen_.sin_addr))==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    
    if (bind(SIPApplicationSock_,(struct sockaddr *)&siSIPApplicationListen_, sizeof siSIPApplicationListen_) == -1) {
        perror("binding datagram socket");
        exit(1);
    }
    
    // Waiting for application port
    while (sipApplicationPort_ == 0) {
        if((pktSize = recvfrom(SIPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
            perror("error while receiving datagram");
            exit(1);
        }
        // Xlite receives data on the same port it sends, so we can use that port
        // for communicating back
        if(siFrom.sin_addr.s_addr == inet_addr("127.0.0.1")) {
            sipApplicationPort_ = ntohs(siFrom.sin_port);
            // Sending back to voip application
            siSIPApplication_.sin_family = AF_INET;
            siSIPApplication_.sin_port = htons(sipApplicationPort_);
            if (inet_aton("127.0.0.1", (struct in_addr *)&(siSIPApplication_.sin_addr))==0) {
                fprintf(stderr, "inet_aton() failed\n");
                exit(1);
            }
            buf[pktSize] = '\0';
            msgProxy.assign(buf);
        }
    }
    std::cout << " Local SIP Port: " << sipApplicationPort_ << std::endl;
    
    fd_set readSet;
    
    int fdMax = std::max(SIPApplicationSock_, SIPProxySock_);    
    
    struct timeval tv;
    tv.tv_sec  = 3600;
    tv.tv_usec = 0;
    
    int checkSIPProxy = false;
    
    // condition checks if new packet has been received and if we still have some missing information
    // to establish rtp connection
    while( (rtpApplicationPort_ == 0) || (rtpProxyPort_ == 0) || (rtpProxyHost_ == "") || (localHost_ == "")) {
        FD_ZERO(&readSet);
        FD_SET(SIPProxySock_,&readSet);    
        FD_SET(SIPApplicationSock_,&readSet); 

        int retval = select(fdMax+1,&readSet,0,0,NULL);

        if(retval < 0) printf("select error\n");
        if(retval>0) {
            if(FD_ISSET(SIPProxySock_,&readSet)) {
                if((pktSize = recvfrom(SIPProxySock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }

                // If we are here local communication must work correctly. We 
                // only need to check the remote one
                if (!checkSIPProxy) {
                    std::cout << "Done." << std::endl;
                    checkSIPProxy = true;
                }
                
                buf[pktSize] = '\0';
              //  printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);
                // set the RTP port and IP if available
                int rtpProxyPort;
                std::string rtpProxyHost;
                std::string localHost;
                if((rtpProxyPort = pktSIP.getSDP()->getRTPPort())!=-1) {
                    rtpProxyPort_ =rtpProxyPort;
                    pktSIP.getSDP()->setRTPPort(rtpInternalPort_);
                    std::cout << " RTP Proxy Port: " << rtpProxyPort_ << std::endl;                    
                }
                if((rtpProxyHost = pktSIP.getSDP()->getRTPIP())!="") {
                    rtpProxyHost_ = rtpProxyHost;
                    pktSIP.getSDP()->setRTPIP("127.0.0.1");
                    std::cout << " RTP Proxy Host: " << rtpProxyHost_ << std::endl;                    
                }
                // Get our local connection ip
                 // if((localHost = pktSIP.getReceivedHost())!="") {
                 //    localHost_ = localHost;
                 //    std::cout << " Local Connection IP: " << localHost_ << std::endl;                    
                 // }

                if(pktSIP.checkMethod("100 Trying")) {
                    std::cout << "100 Trying" << std::endl;
                    pktSIP.setViaHost("127.0.0.1:5063");
////                    pktSIP.setViaReceivedHost(SIP_PROXY_HOST); ??
////                    pktSIP.setViaRport("127.0.0.1:5063"); ??
                }
                else if(pktSIP.checkMethod("401 Unauthorized")) {
                    pktSIP.setViaHost("127.0.0.1:5063");
                }
                else if(pktSIP.checkMethod("404 Not Found")) {
                    pktSIP.setViaHost("127.0.0.1:5063");
                }
                else if(pktSIP.checkMethod("200 OK")) {
                    pktSIP.setViaHost("127.0.0.1:5063");
                    pktSIP.setContactHost("127.0.0.1:5063");
                } 
                else if(pktSIP.checkMethod("INVITE")) {
                    pktSIP.setRequestLineHost("INVITE","127.0.0.1:5063");                    
                    // RTP stuff here
                }
                else if(pktSIP.checkMethod("ACK")) {
                    pktSIP.setRequestLineHost("ACK","127.0.0.1:5063");
                }
                else if(pktSIP.checkMethod("OPTIONS")) {
                    pktSIP.setRequestLineHost("OPTIONS","127.0.0.1:5063");
                    pktSIP.setToHost("127.0.0.1:5063");
                }
                else if(pktSIP.checkMethod("BYE")) {
                    pktSIP.setRequestLineHost("BYE","127.0.0.1:5063");                }
                
                msgApplication = pktSIP.getMsg();
                
                // prepare the message to send
                slen = sizeof(siSIPApplication_);
                if (sendto(SIPApplicationSock_, msgApplication.c_str(), msgApplication.size(),0, (struct sockaddr *) &(siSIPApplication_),slen) == -1) {
                    perror("sending sip to application failed");
                    close(SIPApplicationSock_);
                }
                msgApplication.clear();
            }
            if(FD_ISSET(SIPApplicationSock_,&readSet)) {
                if((pktSize = recvfrom(SIPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                buf[pktSize] = '\0';
              //  printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));       
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);

                // set the RTP port if available
                int rtpApplicationPort;
                if((rtpApplicationPort = pktSIP.getSDP()->getRTPPort())!=-1) {
                    rtpApplicationPort_ = rtpApplicationPort;
                    pktSIP.getSDP()->setRTPPort(rtpExternalPort_);
                    std::cout << " RTP Application Port: " << rtpApplicationPort_ << std::endl;
                }
//                if((pktSIP.getSDP()->getRTPIP())!="" && localHost_!="") {
                if(localHost_!="") {
                    pktSIP.getSDP()->setRTPIP(localHost_);
                    if(pktSIP.checkMethod("PUBLISH")) {
                        pktSIP.setVHost(localHost_);
                    }
                    else if(pktSIP.checkMethod("REGISTER")) {
                        pktSIP.setVHost(localHost_);
                        pktSIP.setMHost(localHost_);
                    }
                    else if(pktSIP.checkMethod("100 Trying")) {
                        pktSIP.setVReceivedHost(SIP_PROXY_HOST);
                        pktSIP.setVRport(8060);                        
                    }
                    else if(pktSIP.checkMethod("180 Ringing")) {
                        pktSIP.setVReceivedHost(localHost_);
                        pktSIP.setMHost(localHost_);
                    }
                    else if(pktSIP.checkMethod("200 OK")) {
                        pktSIP.setVReceivedHost(localHost_);
                        pktSIP.setMHost(localHost_);
                    }
                }

          //      if(pktSIP.checkMethod("REGISTER")) std::cout << "REGISTER" << std::endl;
          //      if(pktSIP.checkMethod("INVITE")) std::cout << "INVITE" << std::endl;
                
                
                msgProxy = pktSIP.getMsg();
                
                
                // std::cout << msgProxy << std::endl << std::endl;
                // prepare the message to send
                slen = sizeof(siSIPProxy_);    
                if (sendto(SIPProxySock_, msgProxy.c_str(), msgProxy.size(),0, (struct sockaddr *) &(siSIPProxy_),slen) == -1) {
                    perror("sending packet to sip proxy failed");
                    close(SIPProxySock_);
                }
                msgProxy.clear();                
            }
        }
    }
    
    RTPApplicationSock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    RTPProxySock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   
    struct addrinfo* rtpAddrInfoResult;
    
    if (getaddrinfo(rtpProxyHost_.c_str(), NULL, NULL, &rtpAddrInfoResult) != 0) {   
        fprintf(stderr, "getaddrinfo() failed\n");
        exit(1);
    }

    std::cout << "Configuring RTP Communication... " << std::endl;    

    // Setting RTP proxy communication
    siRTPProxyListen_.sin_family = AF_INET;
    siRTPProxyListen_.sin_port = htons(rtpExternalPort_);
    siRTPProxyListen_.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(RTPProxySock_,(struct sockaddr *)&siRTPProxyListen_, sizeof siRTPProxyListen_) == -1) {
        perror("binding datagram socket");
        exit(1);
    }

    siRTPProxy_.sin_family = AF_INET;
    siRTPProxy_.sin_port = htons(rtpProxyPort_);
    if (inet_aton(rtpProxyHost_.c_str(), (struct in_addr *)&(siRTPProxy_.sin_addr))==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Setting RTP application communication
    siRTPApplicationListen_.sin_family = AF_INET;
    siRTPApplicationListen_.sin_port = htons(rtpInternalPort_);
    if (inet_aton("127.0.0.1", (struct in_addr *)&(siRTPApplicationListen_.sin_addr))==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if (bind(RTPApplicationSock_,(struct sockaddr *)&siRTPApplicationListen_, sizeof siRTPApplicationListen_) == -1) {
        perror("binding datagram socket");
        exit(1);
    }
    
    
    // Sending back to voip application
    siRTPApplication_.sin_family = AF_INET;
    siRTPApplication_.sin_port = htons(rtpApplicationPort_);
    if (inet_aton("127.0.0.1", (struct in_addr *)&(siRTPApplication_.sin_addr))==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    
    
    
    // Check if we're receiving RTP packets from both sides
    // If yes, create the new thread for connection

    int fds[] = {RTPProxySock_,RTPApplicationSock_,
                 SIPProxySock_,SIPApplicationSock_};
    fdMax = *std::max_element(fds,fds+4);
    
    bool checkProxy = false;
    bool checkLocal = false;
    
    while( !checkProxy || !checkLocal ) {
        FD_ZERO(&readSet);
        FD_SET(RTPProxySock_,&readSet);    
        FD_SET(RTPApplicationSock_,&readSet);
        // SIP needed e.g. for ACK message
        FD_SET(SIPProxySock_,&readSet);    
        FD_SET(SIPApplicationSock_,&readSet);         
        
        int retval = select(fdMax+1,&readSet,0,0,NULL);

        if(retval < 0) printf("select error\n");
        if(retval>0) {
            if(FD_ISSET(RTPProxySock_,&readSet)) {
                if((pktSize = recvfrom(RTPProxySock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                
                checkProxy = true;
                
                // prepare the message to send
                slen = sizeof(siRTPApplication_);
                if (sendto(RTPApplicationSock_, buf, pktSize,0, (struct sockaddr *) &(siRTPApplication_),slen) == -1) {
                    perror("sending rtp to application failed");
                    close(RTPApplicationSock_);
                }
            }
            if(FD_ISSET(RTPApplicationSock_,&readSet)) {
                if((pktSize = recvfrom(RTPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                
                checkLocal = true;
                
                // prepare the message to send
                slen = sizeof(siRTPProxy_);    
                if (sendto(RTPProxySock_, buf, pktSize,0, (struct sockaddr *) &(siRTPProxy_),slen) == -1) {
                    perror("sending packet to rtp proxy failed");
                    close(RTPProxySock_);
                }
            }
            if(FD_ISSET(SIPProxySock_,&readSet)) {
                if((pktSize = recvfrom(SIPProxySock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                
                buf[pktSize] = '\0';
                // printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);

                if((pktSIP.getSDP()->getRTPPort())!=-1) {
                    pktSIP.getSDP()->setRTPPort(rtpInternalPort_);
                }
                if((pktSIP.getSDP()->getRTPIP())!="") {
                    pktSIP.getSDP()->setRTPIP("127.0.0.1");
                }
                msgApplication = pktSIP.getMsg();                
                
                // prepare the message to send
                slen = sizeof(siSIPApplication_);
                if (sendto(SIPApplicationSock_, msgApplication.c_str(), msgApplication.size(),0, (struct sockaddr *) &(siSIPApplication_),slen) == -1) {
                    perror("sending sip to application failed");
                    close(SIPApplicationSock_);
                }
                msgApplication.clear();
            
            }
            if(FD_ISSET(SIPApplicationSock_,&readSet)) {
                if((pktSize = recvfrom(SIPApplicationSock_, buf, BUFSIZE, 0, (struct sockaddr *)&siFrom,(socklen_t*) &slen)) == -1) {
                    perror("error while receiving datagram");
                    exit(1);
                }
                buf[pktSize] = '\0';
                // printf("\nReceived SIP packet from %s:%d\n",inet_ntoa(siFrom.sin_addr), ntohs(siFrom.sin_port));       
                PacketHandler::PacketSIP pktSIP = sipParser.generatePacket(buf);

                // set the RTP port if available
                if((pktSIP.getSDP()->getRTPPort())!=-1) {
                    pktSIP.getSDP()->setRTPPort(rtpExternalPort_);
                }
                if((pktSIP.getSDP()->getRTPIP())!="") {
                    pktSIP.getSDP()->setRTPIP(localHost_);
                }
                msgProxy = pktSIP.getMsg();
                
                // prepare the message to send
                slen = sizeof(siSIPProxy_);    
                if (sendto(SIPProxySock_, msgProxy.c_str(), msgProxy.size(),0, (struct sockaddr *) &(siSIPProxy_),slen) == -1) {
                    perror("sending packet to sip proxy failed");
                    close(SIPProxySock_);
                }
                msgProxy.clear();                
            }
        }
    }
  
    std::cout << "Done." << std::endl;    
 
    pthread_create( &listeningThread, NULL, &Connection::listenOnSockets, (void*) this);        
    
    pthread_join( listeningThread, NULL);    
}

Connection::Connection(const Connection& orig) {
}

Connection::~Connection() {
}

void Connection::write() {
    if(!sout_.str().empty()) {
        dataOutReady_ = true;
//        int dlen = sizeof(dest_);
//        sendto(sock_, sout_.str().c_str(), sout_.str().length(), 0, (struct sockaddr*)&dest_, dlen);
//        sout_.str("");
    }
}

void Connection::read() {

}

void Connection::shutdown() {
#ifdef _WIN32
        closesocket(sock_);
        WSACleanup();
#else
//        close(sock_);
#endif
}

std::istringstream& Connection::getInputStream() {
        return sin_;
}

std::ostringstream& Connection::getOutputStream() {
        return sout_;
}

int Connection::getPort() {
//        return port_;
}

void Connection::setPort(int port) {
//        port_ = port;
//        dest_.sin_port = htons(port_);
}

bool Connection::isClosed() {
        return false;
}

bool Connection::isConnected() {
        return connected_;
}

PacketHandler::Packet Connection::receivePacket() {
    
}

int Connection::sendPacket(PacketHandler::Packet packet) {
    
}
