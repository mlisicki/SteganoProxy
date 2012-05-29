/* 
 * File:   ApplicationManager.cpp
 * Author: mike
 * 
 * Created on January 17, 2012, 12:57 PM
 */

#include <iostream>
#include <fstream>
#include <list>
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

void ApplicationManager::loadConfigurationFromFile(char* filename) {
    Parser::XPath xPath("/configuration/sip_external_port");
    Parser::XMLParser parser(filename, xPath);
    try { 
        parser.program();
    } catch(char const* err) {
        std::cout << err << std::endl;
    }
    
    std::list<std::string> result;
    result = parser.getResult();
    
    for (std::list<std::string>::iterator it=result.begin(); it!=result.end(); ++it)
        std::cout << " " << *it;
    std::cout << std::endl;
}