/*
 * SymbolTable.cpp
 *
 *  Created on: Jul 13, 2012
 *      Author: itamar
 */
#include "SymbolTable.h"
#include <iostream>
#include <algorithm>


SymbolTable* SymbolTable::featuresTable = NULL;
SymbolTable* SymbolTable::classesTable = NULL;

SymbolTable* SymbolTable::FeaturesTable()
{
	if(!featuresTable)
	{
		featuresTable = new SymbolTable();
	}
	return featuresTable;
}

SymbolTable* SymbolTable::ClassesTable()
{
	if(!classesTable)
	{
		classesTable = new SymbolTable();
	}
	return classesTable;
}

ui SymbolTable::addName(string name)
{
	unordered_map<string, ui>::const_iterator value = tableNameId.find(name);
	if(value == tableNameId.end()){
		
		tableNameId.insert(make_pair(name, count));
		tableIdName.insert(make_pair(count, name));
		return count++;
	}

	return value->second;
}

ui SymbolTable::getId(const string &name) const
{
	try{
		return tableNameId.at(name);
	}catch(...){
		cerr << "SymbolTable ERROR, invalid key: " << name << endl;
		throw;
	}
}

const string& SymbolTable::getName(const ui id) const
{
	try{
			return tableIdName.at(id);
	}catch(...){
		cerr << "SymbolTable ERROR, invalid id: " << id << endl;
		throw;
	}
}

void SymbolTable::getTableName(vector<pair<string, ui> > &tableName) const{
	copy(tableNameId.begin(), tableNameId.end(), back_inserter(tableName));
	sort(tableName.begin(), tableName.end());
}

void SymbolTable::clear(void){
	count = 0;
	tableNameId.clear();
	tableIdName.clear();
}

