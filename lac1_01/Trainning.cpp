#include "Trainning.h"
#include "SymbolTable.h"
#include <unordered_set>
#include <algorithm>
#include <list>
#include <cstring>
#include <cstdlib>
#include <cstdio>


void Trainning::readInput(const char* fpath){
	FILE* file=fopen(fpath,"r");
	if(file==NULL) {
		fprintf(stderr,"Training set %s not found.\n\n", fpath);
		exit(-1);
	}

	SymbolTable* featureTable = SymbolTable::FeaturesTable();
	SymbolTable* classesTable = SymbolTable::ClassesTable();
	unordered_set<ui> featureIds;
	const ui bufferSize = 200*1024;
	char line[bufferSize];

	ui tid, classId;
	while(fgets(line, bufferSize, file)){
		classId = 100000;
		processLineInput(line, featureTable, classesTable, tid, classId, featureIds);
		if(classId == 100000){
			fprintf(stderr, "Training: class ID not found\n" );
			throw  string("Class Id not found: ") + line ;
		}

		addTransaction(classId, tid, featureIds);
	}

	fclose(file);
}

/**
 * The class id must start with 0 and be incremented by 1
 * Adds a transaction to the trainning
 * @classId identifier of the class
 * @tid indentifier of the transaction
 * @featureIds identifiers of the features
 * @size size of the vector... this is used because in this case the vector can have more space
 */
void Trainning::addTransaction(ui classId, ui tid, const unordered_set<ui> &featureIds){
	while(classesSupport.size() <= classId){
		classesSupport.push_back(0);
	}
	++classesSupport[classId];
	tidClassMap.push_back(classId);

	unordered_map<ui, Itemset*>::iterator it;
	for(const ui &fid : featureIds){
		it = itemsets.find(fid);
		if(it != itemsets.end()){
			it->second->addTid(tidCount, classId);
		}else{
			Itemset* item = new Itemset();
			item->featureIds.push_back(fid);
			item->addTid(tidCount, classId);
			itemsets[fid] = item;
		}
	}

	//we are not using the real tid. You must save it in a list map
	//if you want to remove some trainng transactions later

	++tidCount;
}

/**
 * Return one projection for each class
 */
Projection* Trainning::geProjection(const unordered_set<ui> &featureIds){
	Projection* projection = new Projection(&cache);
	unordered_map<ui, Itemset*>::const_iterator it;
	for(const ui &fid : featureIds){
		it = itemsets.find(fid);
		if(it != itemsets.end()){
			projection->itemsets.push_back(it->second);
		}
	}

	sort(projection->itemsets.begin(), projection->itemsets.end(), Itemset::Less());
	//projection->itemsets.sort(Itemset::Less());

	return projection;
}


ui Trainning::getMostFrequentClass(void) const{
	return  distance(classesSupport.begin(), max_element(classesSupport.begin(), classesSupport.end()));
}

Trainning::~Trainning(void){
	for(pair<const ui, Itemset*> &p : itemsets){
		delete p.second;
	}
}
