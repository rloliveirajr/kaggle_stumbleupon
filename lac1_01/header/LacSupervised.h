/*
 * LacSupervised.h
 * Lazy Associative Classification. A. Veloso, W. Meira and M. Zaki. ICDM 2006.
 * Demand-Driven Associative Classification. March, 2011. Book.
 * http://www.dcc.ufmg.br/~adrianov/THESIS/phd.pdf
 *
 *  Created on: Jul 18, 2012
 *      Author: itamar
 */

#ifndef LACSUPERVISED_H_
#define LACSUPERVISED_H_

#include "Trainning.h"
#include "AssociationRule.h"
#include "util.h"

class LacSupervised{
public:
	explicit LacSupervised(float pminConfidence, float pminSupport, ui piminSupport, ui pmaxRuleSize, ui pcacheSize)
	:  minConfidence(pminConfidence), minSupport(pminSupport), iminSupport(piminSupport), maxRuleSize(pmaxRuleSize), cacheSize(pcacheSize){};

	float predict(Trainning &trainning, char *ftest, AssociationRule*  associationRule);

private:
	float minConfidence;
	float minSupport;
	ui iminSupport;
	ui maxRuleSize;
	ui cacheSize;
	void printStatistics(Trainning &trainning, ui tid, SymbolTable* classesTable, vector<pair<string, ui> > &classes, ui classId,
			ui prediction, vector<float>& scores, float totalScore, ui nrules);
};


#endif /* LACSUPERVISED_H_ */
