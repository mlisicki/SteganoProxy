/* 
 * File:   Symbol.h
 * Author: Michal Lisicki
 *
 * Created on January 18, 2012, 1:20 PM
 */

#ifndef SYMBOL_H
#define	SYMBOL_H

typedef enum {letter, number, white, sym, nullchar, nextline, equal } SymbolValue;

class Symbol {
public:
    SymbolValue get();
    char getCharacter();
    int set(char c);
    
    bool operator==(SymbolValue other) const;   
private:
    SymbolValue value_;
    char character_;
};

#endif	/* SYMBOL_H */

