/* 
 * File:   XMLSymbol.h
 * Author: Michal Lisicki
 *
 * Created on December 5, 2011, 1:13 AM
 */

#ifndef XMLSymbol_H
#define	XMLSymbol_H

namespace Parser {

// atoms
typedef enum {letter, number, white, slash, qMark, equal, lt, gt, opensym, closesym, sym} XMLSymbolValue;

class XMLSymbol {
public:
    XMLSymbolValue get();
    char getCharacter();
    int set(char c);
    
    bool operator==(XMLSymbolValue other) const;   
private:
    XMLSymbolValue value_;
    char character_;
};

}

#endif	/* XMLSymbol_H */

