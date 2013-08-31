#include "AssociationRule.h"
#include <iomanip>
#include <iostream>


AssociationRule::AssociationRule(float pminConfidence, const ui pmaxRuleSize,  const vector<ui> *ptidClassMap, ui cacheSize)
: minConfidence(pminConfidence), maxRuleSize(pmaxRuleSize), tidClassMap(ptidClassMap), cache(cacheSize), maxRules(500*1024)
{
	featuresTable = SymbolTable::FeaturesTable();
	classesTable = SymbolTable::ClassesTable();
}

void AssociationRule::getKey(const vector<Itemset*>& items, vector<ui> &key){

	Itemset* item = items.front();
	for(ui id : item->featureIds){
		key.push_back(id);
	}
	for(Itemset *i : items){
		key.push_back(i->featureIds.back());
	}
}

void AssociationRule::printRule(const Itemset* i, ui classId, ui support, float confidence) const{
	cout.setf(ios::fixed,ios::floatfield);
	cout << setprecision (4) << "conf: " << confidence << " sup: " << support << " ";
	for(const ui id : i->featureIds){
		cout << featuresTable->getName(id) << " ";
	}
	cout <<"-> " <<  classesTable->getName(classId) << endl;//stats

}
