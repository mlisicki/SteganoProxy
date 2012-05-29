/* 
 * File:   XPath.cpp
 * Author: Michal Lisicki
 * 
 * Created on December 5, 2011, 6:25 PM
 */

#include "XPath.h"
#include <iostream>

XPath::XPath() {
    
}

XPath::XPath(std::string xpath) {
    size_t pos = 0;
    size_t last = 1;
    XPathNode* node;
    XPathNode* nextNode;
    while((pos = xpath.find('/',last))!=std::string::npos) {
        nextNode = new XPathNode(xpath.substr(last,pos-last),elem);
        if(last==1) first_ = nextNode;
        else { 
            node->setNext(nextNode);
            nextNode->setPrev(node);
        }
        node = nextNode;
        last = pos+1;
    }
    if(xpath[last]=='@')
        nextNode = new XPathNode(xpath.substr(last+1),attr);
    else
        nextNode = new XPathNode(xpath.substr(last),elem);
    if(last==1) first_ = nextNode;
    else { 
        node->setNext(nextNode);
        nextNode->setPrev(node);
    }
    last_ = new XPathNode("",end);
    nextNode->setNext(last_);
    last_->setPrev(nextNode);
    current_ = first_;
}

//@TODO destructor clearing the list

XPath::XPath(const XPath& orig) {
    first_ = orig.pathRoot();
    current_ = orig.getCurrent();
    last_ = orig.getLast();
}

XPathNode* XPath::pathRoot() const {
    return first_;
}

XPathNode* XPath::getCurrent() const {
    return current_;
}

XPathNode* XPath::getLast() const {
    return last_;
}

bool XPath::getValid() const {
    return valid_;
}

void XPath::setValid(bool valid) {
    valid_ = valid;
}

int XPath::next() {
    if(current_!=last_) {
        current_ = current_->getNext();
        return 1;
    }
    return 0;
}

int XPath::prev() {
    if(current_!=first_) {
        current_ = current_->getPrev();
        return 1;
    }
    return 0;
}
