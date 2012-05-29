/* 
 * File:   XPathNode.cpp
 * Author: Michal Lisicki
 * 
 * Created on December 5, 2011, 6:25 PM
 */

#include "XPathNode.h"

XPathNode::XPathNode(std::string name, XPNodeType type) { 
    name_ = name;
    type_ = type;
}

std::string XPathNode::getName() {
    return name_;
}

XPNodeType XPathNode::getType() {
    return type_;
}

XPathNode* XPathNode::getNext() {
    return next_;
}

void XPathNode::setNext(XPathNode* xPN) {
    next_ = xPN;
}

XPathNode* XPathNode::getPrev() {
    return prev_;
}

void XPathNode::setPrev(XPathNode* xPN) {
    prev_ = xPN;
}