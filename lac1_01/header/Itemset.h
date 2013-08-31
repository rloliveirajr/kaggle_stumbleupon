/*
 * Itemset.h
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#ifndef ITEMSET_H_
#define ITEMSET_H_

#include "util.h"
#include <vector>

/**
 *An Itemset represents the set of transactions the contains one feature
 */
class Itemset{
public:
	Itemset(): support(0){}
	Itemset(const Itemset &rhs):support(rhs.support), featureIds(rhs.featureIds),  classSuport(rhs.classSuport)
	{}
	ui support;
	vector<ui> featureIds;
	vector<ui> classSuport;
	vector<ui> tids;

	Itemset& operator=(const Itemset& rhs){
		this->support = rhs.support;
		this->featureIds.assign(rhs.featureIds.begin(), rhs.featureIds.end());
		this->classSuport.assign(rhs.classSuport.begin(), rhs.classSuport.end());

		return *this;
	}

	void addTid(ui tid, ui classid, bool storage=true){
		++support;
		if(storage){
			tids.push_back(tid);
		}
		while(classSuport.size() <= classid){classSuport.push_back(0);}
		classSuport[classid] += 1;
	}

	ui getSupport(void) const{ return support;}
	ui getSupport(ui classId) const{return classId < classSuport.size()? classSuport[classId] : 0;}

	class Less {
	public:
		bool operator()(const Itemset *lhs, const Itemset *rhs) const{
			if(lhs->getSupport() == rhs->getSupport()){
				return lhs->featureIds.front() < rhs->featureIds.front();
			}

			return lhs->getSupport() < rhs->getSupport();
		}
	};


};



#endif /* ITEMSET_H_ */
