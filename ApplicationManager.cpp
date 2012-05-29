/* 
 * File:   ApplicationManager.cpp
 * Author: mike
 * 
 * Created on January 17, 2012, 12:57 PM
 */

#include <iostream>

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

std::string ApplicationManager::loadConfigurationFromFile(char* text) {
    
}