/* 
 * File:   Parser.cpp
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 4:13 AM
 */

#include "XMLParser.h"
#include <stdlib.h>
#include <iostream>

using namespace Parser;

XMLParser::XMLParser(char* file, XPath xPath) {
    scanStream_.open(file, std::fstream::in);
    if(!scanStream_.is_open()) {
        fprintf(stderr, "Couldn't open XML file\n");
        fprintf(stderr, "\nUsage: XMLValuesGetter xmlfile xpathfile\n");
        exit(EXIT_FAILURE);
    }
    xPath_ = xPath;
    xPath_.setValid(true);
    next();
}

XMLParser::~XMLParser() {
    scanStream_.close();
}

// get the next symbol
void XMLParser::next() { 
    char c;
    scanStream_.get(c);
    if(!symbol_.set(c)) throw "Unexpected symbol";
}

int XMLParser::accept(SymbolValue atom) { 
    if(symbol_ == atom) {
        next();
        return 1;
    } else { 
        throw "Syntax error";
    }
    return 0;
}

void XMLParser::element() {
    std::string tagName = startTag();
    if(symbol_==slash) return;
    // XPath - go deeper only if element fits current xPath name
    if(!xPath_.getValid()) {
        content();
    } else if(xPath_.getCurrent()->getName()==tagName && 
            (xPath_.getCurrent()->getType()==elem || 
            xPath_.getCurrent()->getType()==end)) {
        // change current element to the next one
         xPath_.next();
         content();
         // and come back before end
         xPath_.prev();
    } else {
        xPath_.setValid(false);
        content();
        xPath_.setValid(true);
    }
    endTag();
}

std::string XMLParser::startTag() {
    std::string tagName;
    accept(opensym);
    if(!(symbol_==slash)) {
        tagName = name();
        if(symbol_==white) s();
        while(!(symbol_==closesym)) {
             attribute();
             if(symbol_==white) s();
        }
        accept(closesym);
    }
    return tagName;
}

void XMLParser::endTag() {
    if(symbol_==opensym)
        accept(opensym);
    accept(slash);    
    name();
    accept(closesym);
}

void XMLParser::content() {
    std::string text;
    while(!(symbol_==slash)) {
        while(true) {
            if(symbol_==white) {
                text += symbol_.getCharacter();
                accept(white);
            } else if(symbol_==letter || symbol_==number || symbol_==sym) text += ident();
            else break;
        }
        element();
        while(true) {
            if(symbol_==white) {
                text += symbol_.getCharacter();
                accept(white);
            } else if(symbol_==letter || symbol_==number || symbol_==sym) text += ident();
            else break;
        }
        if(xPath_.getCurrent()==xPath_.getLast()) {
            output_.push_back(text);
        }
        //@TODO what if there will be '/' in a text
    }
}

std::string XMLParser::ident() {
    std::string identString;
    do {
        identString += symbol_.getCharacter();
        if(symbol_==number) accept(number);
        else if(symbol_==letter) accept(letter);
        else if(symbol_==sym) accept(sym);
        else break;
    } while(true);
    identString.erase(identString.end()-1);
    return identString;
}

void XMLParser::attribute() {
    std::string attName = name();
    eq();
    std::string value = attValue();
    xPath_.next();
    if(xPath_.getCurrent()->getName()==attName && 
       xPath_.getCurrent()->getType()==attr &&
       xPath_.getValid()) {
       output_.push_back(value);
    }
    xPath_.prev();
}

std::string XMLParser::attValue() {
    accept(qMark);        
    std::string value = name();
    accept(qMark);
    return value;
}

void XMLParser::s() {
    while(symbol_ == white)
        accept(white);
}

std::string XMLParser::name() {
    return ident();
}

void XMLParser::eq() {
    s();
    accept(equal);
    s();
}

void XMLParser::headerTag() {
    accept(opensym);
    accept(sym);
    name();
    if(symbol_==white) s();
    while(!(symbol_==sym)) {
        attribute();
        if(symbol_==white) s();
    }
    accept(sym);
    accept(closesym);
    s();
}

void XMLParser::program() {
    headerTag();
    element();
}

std::list<std::string>& XMLParser::getResult() {
    return output_;
}
