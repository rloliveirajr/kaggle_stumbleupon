/*
 * util.h
 *
 *  Created on: Jul 13, 2012
 *      Author: itamar
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <list>
#include <unordered_set>
#include "SymbolTable.h"
#include "FSBAllocator.hh"

using namespace std;

typedef unsigned int ui;
typedef unsigned long long ull;

typedef list<ui, FSBAllocator<ui> > listUi;

template<typename Container> void printSequence(const Container &c);
void processLineInput(char* line, SymbolTable* featuresTable, SymbolTable* classesTable, ui &tid, ui &classId, unordered_set<ui> &featureIds);

#define WATCH(x) cout << #x" = " << x << "\n"

const float EPS = 1e-9;
inline int cmp(float x, float y = 0.0, float tol = EPS)
{
    return ( x <= y + tol ) ? ( x + tol < y ) ? -1 : 0 : 1;
}


void tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ");
#endif /* UTIL_H_ */
