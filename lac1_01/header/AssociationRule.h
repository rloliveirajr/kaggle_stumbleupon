/*
 *
 *  Created on: Jul 14, 2012
 *      Author: itamar
 */

#ifndef ASSOCIATIONRULE_H_
#define ASSOCIATIONRULE_H_

#include <list>
#include <vector>
#include <set>
#include "util.h"
#include "Diffset.h"
#include "SymbolTable.h"
#include "Projection.h"
#include "Cache.h"

class AssociationRule{

public:

	class RulesResult{
		public:
		explicit RulesResult(){}
		explicit RulesResult(ui nclasses): classesConfidence(nclasses, 0.0f), classesNRules(nclasses, 0){}
		RulesResult(const RulesResult &rhs): classesConfidence(rhs.classesConfidence), classesNRules(rhs.classesNRules){}

			vector<float> classesConfidence;
			vector<ui> classesNRules;

			RulesResult& operator=(const RulesResult& rhs){
				this->classesConfidence = rhs.classesConfidence;
				this->classesNRules = rhs.classesNRules;
				return *this;
			}

			RulesResult& operator+=(const RulesResult& rhs){
				for(ui i = 0; i < this->classesConfidence.size(); ++i){
					this->classesConfidence[i] += rhs.classesConfidence[i];
					this->classesNRules[i] += rhs.classesNRules[i];
				}

				return *this;
			}

			float score(const ui classId) const{
				return classesNRules[classId] == 0 ?  0.0f :  classesConfidence[classId]/static_cast<float>(classesNRules[classId]);
			}
	};


	explicit AssociationRule(float pminConfidence, const ui pmaxRuleSize,  const vector<ui> *ptidClassMap, ui cacheSize);


	virtual RulesResult induceRules(Projection  *projection, float pminSupport, ui iminSupport, ui nclasses) = 0;
	virtual ~AssociationRule(){}

protected:
	float minConfidence;
	ui minSupport;
	ui maxRuleSize;
	SymbolTable* featuresTable;
	SymbolTable* classesTable;
	const vector<ui> *tidClassMap;//[tid] = classId
	Cache<vector<ui>, RulesResult> cache;
	const ui maxRules;
	ui totalRules;

	void getKey(const vector<Itemset*>&, vector<ui> &key);
	void printRule(const Itemset*, ui classId, ui support, float confidence) const;
};


#endif /* ASSOCIATIONRULE_H_ */
