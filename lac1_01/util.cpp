/*
 * util.cpp
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <string>

#include "util.h"

template<typename Container> void printSequence(const Container &c)
{
	for(typename Container::const_iterator it = c.begin(); it != c.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
}
//
//void processLineInput(char* line, SymbolTable* featuresTable, SymbolTable* classesTable, ui &tid, ui &classId, unordered_set<ui> &featureIds){
//	try {
//		istringstream iss(line);
//		iss >> tid;
//		string item;
//		featureIds.clear();
//		while(iss >> item) {
//			if(item[0] == 'w'){//feature
//				ui id = featuresTable->addName(item);
//				featureIds.insert(id);
//			}else if(item[0] == 'C'){//class
//				string c = item.substr(6);
//				classId = classesTable->addName(c);
//
//			}else{
//				throw string("Invalid format: ") + line;
//			}
//		}
//	} catch (string &e) {
//		fprintf(stderr, "ERRO ao processar a linha: %s. %s\n", line, e.c_str());
//		assert(false);
//	}
//
//}
//



void tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void processLineInput(char* line, SymbolTable* featuresTable, SymbolTable* classesTable, ui &tid, ui &classId, unordered_set<ui> &featureIds){
	try {
		if(!line || iscntrl(line[0])){
			throw string("Empty Line!");
		}
		char* item=strtok(line, " \t\n");
		tid = atoi(item);
		item=strtok(NULL, " \t\n");
		featureIds.clear();
		while(item!=NULL) {
			if(item[0] == 'w'){//feature
				featureIds.insert(featuresTable->addName(item));
			}else if(item[0] == 'C'){//class
				char *c = item + 6;//CLASS=
				classId = classesTable->addName(c);

			}else{
				throw string("Invalid format: ") + line;
			}
			item=strtok(NULL, " \t\n");
		}
	} catch (string &e) {
		fprintf(stderr, "ERRO ao processar a linha: %sMessage: %s\n", line, e.c_str());
		assert(false);
	}

}





