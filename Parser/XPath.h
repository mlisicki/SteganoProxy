/* 
 * File:   XPath.h
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 4:13 AM
 */

#ifndef XPATH_H
#define	XPATH_H

#include <string>
#include "XPathNode.h"

class XPath {
public:
        XPath();
        XPath(std::string xpath);
        XPath(const XPath& orig);
        
        XPathNode* pathRoot() const;
        XPathNode* getCurrent() const;
        XPathNode* getLast() const;
        bool getValid() const;
        void setValid(bool valid);
        int next();
        int prev();
private:
        XPathNode* first_;
        XPathNode* last_;
	XPathNode* current_;
        bool valid_; // check if path is valid up to this point
};

#endif	/* XPATH_H */
