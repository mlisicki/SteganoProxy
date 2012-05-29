/* 
 * File:   Parser.h
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 4:13 AM
 */

#ifndef PARSER_H
#define	PARSER_H

#include <fstream>
#include "XPath.h"
#include "Symbol.h"

class XMLParser {
public:
    XMLParser(char* file, XPath xPath);
    virtual ~XMLParser();

    void next();
    int accept(SymbolValue atom);
    void element();
    std::string startTag();
    void endTag();
    void content();
    std::string ident();
    void attribute();
    std::string attValue();
    void s();
    std::string name();
    void eq();
    void headerTag();     
    void program();
    
//    void program();
//    friend class Synchronize;
private:
    Symbol symbol_;
    Symbol back_;
    std::fstream scanStream_;
    XPath xPath_;
};

#endif	/* PARSER_H */

