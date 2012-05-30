/* 
 * File:   ApplicationManager.cpp
 * Author: mike
 * 
 * Created on January 17, 2012, 12:57 PM
 */

#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>
#include "Parser/XPath.h"
#include "Parser/XMLParser.h"

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager() {
}

ApplicationManager::ApplicationManager(const ApplicationManager& orig) {
}

ApplicationManager::~ApplicationManager() {
}

std::string ApplicationManager::readLine(char* text) {
    char* ch = text;
    std::string out;
    while(*ch!='\n' && *ch!='\r' && *ch!='\0') {
        out += *ch;
        ch++;
    }
    return out;
}

ConnectionConfiguration ApplicationManager::loadConfigurationFromFile(std::string filename) {
    ConnectionConfiguration cconf;
    Parser::XPath* xPath;
    Parser::XMLParser* parser;
    
    xPath = new Parser::XPath("/configuration/sip_external_port");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.sipExternalPort = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/sip_internal_port");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.sipInternalPort = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/sip_proxy_port");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.sipProxyPort = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/rtp_external_port");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.rtpExternalPort = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/rtp_internal_port");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.rtpInternalPort = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/sip_proxy_host");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.sipProxyHost = parser->getResult().front();
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/local_ip");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.localIP= parser->getResult().front();
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/rtp_mean");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.rtpMean = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    xPath = new Parser::XPath("/configuration/rtp_std_dev");
    parser = new Parser::XMLParser(filename.c_str(), *xPath);
    try { parser->program(); } 
    catch(char const* err) { std::cout << err << std::endl; }
    cconf.rtpStdDev = atoi(parser->getResult().front().c_str());
    delete xPath;
    delete parser;
    
    return cconf;
}