/* 
 * File:   ApplicationManager.h
 * Author: Michal Lisicki
 *
 * Created on January 17, 2012, 12:57 PM
 */

#ifndef APPLICATIONMANAGER_H
#define	APPLICATIONMANAGER_H

#include <string>

class ApplicationManager {
public:
    static ApplicationManager& getInstance() {
        static ApplicationManager instance;
        return instance;
    }

    std::string readLine(char* text);
private:
    std::string STPProxy;
    int connectionPort;

    ApplicationManager();
    ApplicationManager(const ApplicationManager& orig);
    virtual ~ApplicationManager();
};

#endif	/* APPLICATIONMANAGER_H */
