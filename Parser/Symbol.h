/* 
 * File:   Symbol.h
 * Author: mike
 *
 * Created on December 5, 2011, 1:13 AM
 */

#ifndef SYMBOL_H
#define	SYMBOL_H

// atoms
typedef enum {letter, number, white, slash, qMark, equal, lt, gt, opensym, closesym, sym} SymbolValue;

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

