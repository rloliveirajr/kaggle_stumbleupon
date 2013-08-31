/*
 * Trainning.h
 *
 *  Created on: Jul 16, 2012
 *      Author: itamar
 */

#ifndef TRAINNING_H_
#define TRAINNING_H_

#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Itemset.h"
#include "util.h"
#include "Projection.h"
#include "Cache.h"


class Trainning{
private:
	vector<ui> classesSupport;
	vector<ui> tidClassMap;//[tid] = classId
	unordered_map<ui, Itemset*> itemsets;
	ui tidCount;
	Cache<vector<ui>, ui> cache;
public:
	explicit Trainning(ui cacheSize):tidCount(0), cache(cacheSize){}
	~Trainning(void);
	void readInput(const char* fpath);
	void addTransaction(ui classId, ui tid, const unordered_set<ui> &featureIds);
	Projection* geProjection(const unordered_set<ui> &featureIds);
	ui getNumberOfClasses(void)const{return classesSupport.size();}
	ui getMostFrequentClass(void) const;
	float getClassProb(ui i){return static_cast<float>(classesSupport[i])/static_cast<float>(tidCount);}
	vector<ui> const* getTidClassMap(void) const{ return &tidClassMap;}
};

#endif /* TRAINNING_H_ */
