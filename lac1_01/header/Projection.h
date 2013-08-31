/*
 * Projection.h
 *
 *  Created on: Jul 18, 2012
 *      Author: itamar
 */

#ifndef PROJECTION_H_
#define PROJECTION_H_

#include "Itemset.h"
#include "Cache.h"
#include <set>

class Projection{
public:
	explicit Projection(Cache<vector<ui>, ui>* pcache): cache(pcache){}
	vector<Itemset*> itemsets;

	/**
	 * Counts the number of unique transactions in the itemsets
	 */
	ui getSize(){
		ui size = 0;
		vector<ui> key;
		for(Itemset* i : itemsets){
			key.push_back(i->featureIds.front());
		}

		if(!cache->get(key, size)){
			unordered_set<ui> set;
			for(Itemset* itemset : itemsets){
				for(const ui &tid : itemset->tids ){
					if(set.find(tid) == set.end()){
						++size;
						set.insert(tid);
					}
				}
			}
			cache->insert(key, size);
		}
		return size;
	}
private:
	Cache<vector<ui>, ui>* cache;
};

#endif /* PROJECTION_H_ */
