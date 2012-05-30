/* 
 * File:   XPathNode.h
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 4:13 AM
 */

#ifndef XPATHNODE_H
#define	XPATHNODE_H

#include <string>

namespace Parser {

enum XPNodeType {elem, attr, end};

class XPathNode {   
public:
        XPathNode(std::string name, XPNodeType type);
        std::string getName();
        XPNodeType getType();
        XPathNode* getNext();
        void setNext(XPathNode* xPN);
        XPathNode* getPrev();
        void setPrev(XPathNode* xPN);
private:                
	XPathNode* next_;
	XPathNode* prev_;
	XPNodeType type_;
        std::string name_; 
};

}

#endif	/* XPATHNODE_H */
