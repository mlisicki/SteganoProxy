/* 
 * File:   ConnectionConfiguration.h
 * Author: Michal Lisicki
 *
 * Created on May 30, 2012, 1:14 AM
 */

#ifndef CONNECTIONCONFIGURATION_H
#define	CONNECTIONCONFIGURATION_H

#include <string>

class ConnectionConfiguration {
public:
    ConnectionConfiguration();
    ConnectionConfiguration(const ConnectionConfiguration& orig);
    virtual ~ConnectionConfiguration();
    
    std::string sipProxyHost;
    int sipExternalPort;
    int sipInternalPort;
    int sipProxyPort;
    int rtpExternalPort;
    int rtpInternalPort;
    std::string localIP;
private:

};

#endif	/* CONNECTIONCONFIGURATION_H */

