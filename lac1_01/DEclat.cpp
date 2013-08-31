/*
 * DEclat.cpp
 *
 * Fast vertical mining using diffsets, Zaki, ACM SIGKDD 2003
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#include <iomanip>

#include "DEclat.h"
#include "Diffset.h"
#include "util.h"

DEclat::DEclat(float pminConfidence, const ui pmaxRuleSize, const vector<ui> *ptidClassMap, ui cacheSize)
: AssociationRule(pminConfidence, pmaxRuleSize, ptidClassMap, cacheSize)
{

}

AssociationRule::RulesResult DEclat::induceRules(Projection  *projection, float pminSupport, ui iminSupport, ui nclasses){
	RulesResult result(nclasses);
	if(!projection->itemsets.size()){
		return result;
	}

	minSupport = iminSupport ? iminSupport : ui(projection->getSize() *pminSupport + 0.5f);
	vector<ui> key;
	totalRules = 0;
	this->AssociationRule::getKey(projection->itemsets, key);

	if(cache.get(key, result)){
		return result;
	}

	if(maxRuleSize == 1){return result;}

	vector<Itemset*>::const_iterator end = projection->itemsets.end();
	for(vector<Itemset*>::const_iterator i = projection->itemsets.begin(); i != end; ++i){
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
		if(totalRules >= maxRules -1){break;}
		if(maxRuleSize == 2){continue;}

		vector<Diffset*> equivalenceClasses;
		vector<Itemset*>::const_iterator j = i;

		for(++j; j != end; ++j){
			Diffset* d = new Diffset(**i, **j, *tidClassMap);
			if(d->support >= minSupport){
				equivalenceClasses.push_back(d);
			}else{
				delete d;
			}
		}
		if(equivalenceClasses.size() == 0){continue;}

		result += induceRules(equivalenceClasses, 2, nclasses);
		for(Diffset* e : equivalenceClasses){
			delete e;
		}
	}

	cache.insert(key, result);

	return result;
}

AssociationRule::RulesResult DEclat::induceRules(const vector<Diffset*>  &equivalenceClasses, const ui ruleSize, ui nclasses){
	RulesResult result(nclasses);

	vector<Diffset*>::const_iterator end = equivalenceClasses.end();
	for(vector<Diffset*>::const_iterator i = equivalenceClasses.begin(); i != end; ++i){
		ui sup = (*i)->getSupport();
		if(sup < minSupport){continue;}
		float itemsetSupport = (static_cast<float>(sup) + EPS);
		for(ui classId = 0; classId < nclasses; classId++){
			float confidence = static_cast<float>((*i)->getSupport(classId))/itemsetSupport;
			if(cmp(confidence, minConfidence) > -1){
				//printRule(*i, classId, (*i)->getSupport(classId), confidence);
				result.classesConfidence[classId] += confidence;
				result.classesNRules[classId] += 1;
				++totalRules;
			}
		}

		if(totalRules >= maxRules -1){break;}
		if(maxRuleSize == ruleSize + 1){continue;}

		vector<Diffset*> equivalenceClasses;
		vector<Diffset*>::const_iterator j = i;
		for(++j; j != end; ++j){
			Diffset* d = new Diffset(**i, **j, *tidClassMap);
			if(d->support >= minSupport){
				equivalenceClasses.push_back(d);
			}else{
				delete d;
			}
		}
		if(equivalenceClasses.size() == 0){continue;}
		result += induceRules(equivalenceClasses, ruleSize+1, nclasses);
		for(Diffset* e : equivalenceClasses){
				delete e;
		}
	}

	return result;
}

void DEclat::getKey(const vector<Diffset*>& items, int level, set<int> &key){
	key.insert(-level);
	Diffset* d = items.front();
	for(ui id :  d->featuresIds){
		key.insert(id);
	}

	for(Diffset* d1 : items){
		key.insert(d1->tids.back());
	}

}


void DEclat::printRule(const Diffset* i, ui classId, ui support, float confidence) const{
	cout.setf(ios::fixed,ios::floatfield);
	cout << setprecision (4) << "conf: " << confidence << " sup: " << support << " ";

	for(const ui id : i->featuresIds){
		cout << featuresTable->getName(id) << " ";
	}

	cout << "-> " <<  classesTable->getName(classId) << endl;//stats
}


