/*
 * Diffset.cpp
 *   Fast vertical mining using diffsets, Zaki, ACM SIGKDD 2003
 *
 *  This data scructure only keeps track of differences in the
 *  transactions ids (tids) of a candidate pattern from its generating
 *  frequent patterns.
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#include <algorithm>
#include "Diffset.h"

/**
 *Gets the diffenrence itemsetA - itemsetB. The two lists must be ordered.
 */
Diffset::Diffset(const Itemset &itemsetA, const Itemset &itemsetB, const vector<ui>& tidClassMap){

	this->classSuport = vector<ui>(itemsetA.classSuport.size(), 0);
	difference(itemsetA.tids, itemsetB.tids, tidClassMap);

	for(ui i = 0; i < this->classSuport.size(); ++i){
		this->classSuport[i] = itemsetA.classSuport[i] - this->classSuport[i];
	}

	this->support = itemsetA.tids.size() - this->tids.size();
	this->featuresIds.assign(itemsetA.featureIds.begin(), itemsetA.featureIds.end());
	this->featuresIds.push_back(itemsetB.featureIds.back());

}

/**
 * Gets the difference between two diff sets B - A
 */
Diffset::Diffset(const Diffset &diffsetA, const Diffset &diffsetB, const vector<ui>& tidClassMap){

	this->classSuport = vector<ui>(diffsetA.classSuport.size(), 0);
	difference(diffsetB.tids, diffsetA.tids, tidClassMap);

	for(ui i = 0; i < this->classSuport.size(); ++i){
			this->classSuport[i] = diffsetA.classSuport[i] - this->classSuport[i];
	}


	this->support = diffsetA.support -  this->tids.size();
	this->featuresIds.assign(diffsetA.featuresIds.begin(), diffsetA.featuresIds.end());
	this->featuresIds.push_back(diffsetB.featuresIds.back());
}

void Diffset::difference(const vector<ui> &a, const vector<ui> &b, const vector<ui>& tidClassMap){
	vector<ui>::const_iterator aIt=a.begin(), aEnd= a.end();
	vector<ui>::const_iterator bIt=b.begin(), bEnd= b.end();
	for(; aIt != aEnd; ++aIt){
		while(bIt != bEnd && *bIt < *aIt){ ++bIt;}
		if(bIt == bEnd || *aIt != *bIt){
			this->tids.push_back(*aIt);
			this->classSuport[tidClassMap[*aIt]] += 1;
		}
	}
}
