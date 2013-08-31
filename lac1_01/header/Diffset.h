/*
 * Diffset.h
 *  Fast vertical mining using diffsets, Zaki, ACM SIGKDD 2003
 *
 *  This data scructure only keeps track of differences in the
 *  transactions ids (tids) of a candidate pattern from its generating
 *  frequent patterns.
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#ifndef DIFFSET_H_
#define DIFFSET_H_

#include <vector>
#include "util.h"
#include "Itemset.h"



class Diffset{
public:
	explicit Diffset(const Itemset &itemsetA, const Itemset &itemsetB, const vector<ui>& tidClassMap);
	Diffset(const Diffset &diffsetA, const Diffset &diffsetB, const vector<ui>& tidClassMap);
	ui getSupport(void) const{ return support;}
	ui getSupport(ui classId) const{return classId < classSuport.size()? classSuport[classId] : 0;}

//private:
	ui support;
	vector<ui> classSuport;
	vector<ui> tids;
	vector<ui> featuresIds;
	void difference(const vector<ui> &a, const vector<ui> &b, const vector<ui>& tidClassMap);

};


#endif /* DIFFSET_H_ */
