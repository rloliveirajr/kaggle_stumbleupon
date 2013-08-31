/*
 * DEclat.h
 * Fast vertical mining using diffsets, Zaki, ACM SIGKDD 2003
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#ifndef DECLAT_H_
#define DECLAT_H_

#include <list>
#include <vector>
#include "util.h"
#include "Diffset.h"
#include "SymbolTable.h"
#include "Projection.h"
#include "Cache.h"
#include "AssociationRule.h"

class DEclat: public AssociationRule{

public:

	explicit DEclat(float minConfidence, const ui pmaxRuleSize,  const vector<ui> *ptidClassMap, ui cacheSize);

	virtual AssociationRule::RulesResult induceRules(Projection  *projection, float pminSupport, ui iminSupport, ui nclasses);

private:
	AssociationRule::RulesResult induceRules(const vector<Diffset*>  &equivalenceClasses, const ui ruleSize, ui nclasses);

	void getKey(const vector<Diffset*>&, int level, set<int> &key);
	void printRule(const Diffset*, ui classId, ui support, float confidence) const;
};


#endif /* DECLAT_H_ */
