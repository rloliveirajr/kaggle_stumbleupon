/*
 * SymbolTable.h
 *
 *  Created on: Jul 13, 2012
 *      Author: itamar
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
#include <vector>
#include <unordered_map>
//can not include "util.h"
typedef unsigned int ui;

using namespace std;

/**
 * Class to associate name with ids
 */
class SymbolTable{
public:
	static SymbolTable* FeaturesTable();
	static SymbolTable* ClassesTable();
	ui addName(string name);
	ui getId(const string &name) const;
	ui size(void)const{return tableNameId.size();}
	const string& getName(const ui id) const;
	void clear(void);
	void getTableName(vector<pair<string, ui> > &tableName) const;




private:
	explicit SymbolTable():count(0){};
	explicit SymbolTable(SymbolTable const&){}//copy constructor
	SymbolTable& operator=(SymbolTable const&){  return *this;};//assignement
	unordered_map<string, ui> tableNameId;
	unordered_map<ui, string> tableIdName;
	ui count;

	static SymbolTable* featuresTable;
	static SymbolTable* classesTable;

};

#endif /* SYMBOLTABLE_H_ */
