/*
 * Eclat.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: itamar
 */

#include "Eclat.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

Eclat::Eclat(float pminConfidence, const ui pmaxRuleSize, const vector<ui> *ptidClassMap, ui cacheSize)
: AssociationRule(pminConfidence, pmaxRuleSize, ptidClassMap, cacheSize), ruleCache(cacheSize)
{

}

AssociationRule::RulesResult Eclat::induceRules(Projection  *projection, float pminSupport, ui iminSupport, ui nclasses){
	RulesResult result(nclasses);
	if(!projection->itemsets.size()){
		return result;
	}
	unordered_map<ui, Itemset const*> database;
	minSupport = iminSupport ? iminSupport : ui(projection->getSize() *pminSupport + 0.5f);
	vector<ui> key;
	totalRules = 0;
	this->AssociationRule::getKey(projection->itemsets, key);

	if(cache.get(key, result)){
		return result;
	}

	if(maxRuleSize == 1){return result;}

	vector<Itemset*>* equiClass = new vector<Itemset*>();
	for(Itemset const*item : projection->itemsets){
		if(item->getSupport() < minSupport) {continue;}
		database[item->featureIds.front()] = item;
		Itemset* newItem = new Itemset(*item);
		equiClass->push_back(newItem);
	}

	queue<vector<Itemset*>* > equivalenceClasses;
	equivalenceClasses.push(equiClass);

	induceRules(equivalenceClasses, nclasses, database, result);

	cache.insert(key, result);
	return result;
}


void Eclat::induceRules(queue<vector<Itemset*> *>& equivalenceClasses, ui nclasses, unordered_map<ui, Itemset const*>& database,  RulesResult& result) {
	vector<Itemset*>* equiClass;
	while (!equivalenceClasses.empty()) {
		equiClass = equivalenceClasses.front();
		equivalenceClasses.pop();
		vector<Itemset*>::const_iterator end = equiClass->end();
		for (vector<Itemset*>::const_iterator i = equiClass->begin(); i != end; ++i) {
			//evalute rule
			if((*i)->getSupport() < minSupport){continue;}
			float itemsetSupport = (static_cast<float>((*i)->getSupport()) + EPS);

			for(ui classId = 0; classId < nclasses; classId++){
				float confidence = static_cast<float>((*i)->getSupport(classId))/itemsetSupport;
				if(cmp(confidence, minConfidence) > -1){
					//this->AssociationRule::printRule(*i, classId, (*i)->getSupport(classId), confidence);
					result.classesConfidence[classId] += confidence;
					result.classesNRules[classId] += 1;
					++totalRules;
				}
			}

			//TODO:review the rules count
			if(totalRules >= maxRules -1){break;}
			if(maxRuleSize == (*i)->featureIds.size() + 1){continue;}

			vector<Itemset*>* newEquiClass = new vector<Itemset*>();
			vector<Itemset*>::const_iterator j = i;

			for(++j; j != end; ++j){
				Itemset* d = getItemset(**i, **j, database);
				if(d->getSupport() >= minSupport){
					newEquiClass->push_back(d);
				}else{
					delete d;
				}
			}
			if(newEquiClass->size() > 0){
				equivalenceClasses.push(newEquiClass);
			}else{
				delete newEquiClass;
			}
		}

		for(Itemset* i : *equiClass){
			delete i;
		}
		delete equiClass;
	}

}

Itemset* Eclat::getItemset(const Itemset& i, const Itemset& j, unordered_map<ui, Itemset const*>& database) {
	Itemset* ret = new Itemset();
	ret->featureIds.assign(i.featureIds.begin(), i.featureIds.end());
	ret->featureIds.push_back(j.featureIds.back());
	if(ruleCache.get(ret->featureIds, *ret)){
		return ret;
	}


	listUi tids;

	vector<ui>::const_iterator fid = i.featureIds.begin();
	Itemset const* itemset = database[*fid];
	tids.assign(itemset->tids.begin(), itemset->tids.end());
	for(++fid; fid != i.featureIds.end(); ++fid){
		 itemset = database[*fid];
		 calcIntersection(tids, itemset->tids);
	}
	itemset = database[j.featureIds.back()];
	calcIntersection(tids, itemset->tids);
	for(ui id : tids){
		ret->addTid(id, tidClassMap->at(id), false );
	}

	ruleCache.insert(ret->featureIds, *ret);

	return ret;
}

void Eclat::calcIntersection(listUi &a, const vector<ui>  &b){
	listUi::iterator aStart = a.begin();
	listUi::iterator aEnd = a.end();
	vector<ui> ::const_iterator bStart = b.begin();
	vector<ui> ::const_iterator bEnd = b.end();
	while(aStart != aEnd){
		while(bStart != bEnd && *bStart < *aStart){++bStart;}
		if(bStart != bEnd && *aStart == *bStart){
			++aStart;
		}else{
			a.erase(aStart++);
		}
	}
}


