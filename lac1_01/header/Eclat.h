/*
 * Eclat.h
 * Mohammed J Zaki , Srinivasan Parthasarathy , Mitsunori Ogihara , Wei Li,
 * New Algorithms for Fast Discovery of Association Rules,
 * University of Rochester, Rochester, NY, 1997
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#ifndef ECLAT_H_
#define ECLAT_H_

#include <list>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include "util.h"
#include "SymbolTable.h"
#include "Projection.h"
#include "Cache.h"
#include "AssociationRule.h"

class Eclat: public AssociationRule{

public:
	explicit Eclat(float minConfidence, const ui pmaxRuleSize,  const vector<ui> *ptidClassMap, ui cacheSize);
	virtual AssociationRule::RulesResult induceRules(Projection  *projection, float pminSupport, ui iminSupport, ui nclasses);

private:
	Cache<vector<ui>, Itemset> ruleCache;
	void induceRules(queue<vector<Itemset*> *>& equivalenceClasses, ui nclasses, unordered_map<ui, Itemset const*>& database, RulesResult& result);
	Itemset* getItemset(const Itemset& i, const Itemset& j, unordered_map<ui, Itemset const*>& database);
	void calcIntersection(listUi &a, const vector<ui>  &b);

};


#endif /* ECLAT_H_ */
