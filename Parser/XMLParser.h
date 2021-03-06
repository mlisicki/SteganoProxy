/* 
 * File:   Parser.h
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 4:13 AM
 */

#ifndef PARSER_H
#define	PARSER_H

#include <fstream>
#include <list>
#include "XPath.h"
#include "XMLSymbol.h"

namespace Parser {

class XMLParser {
public:
    XMLParser(char* file, XPath xPath);
    virtual ~XMLParser();

    void next();
    int accept(XMLSymbolValue atom);
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
    
    std::list<std::string>& getResult();
//    void program();
//    friend class Synchronize;
private:
    XMLSymbol symbol_;
    XMLSymbol back_;
    std::fstream scanStream_;
    XPath xPath_;
    
    std::list<std::string> output_;
};

}

#endif	/* PARSER_H */

