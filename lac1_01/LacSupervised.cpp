/*
 * LacSupervised.cpp
 * Lazy Associative Classification. A. Veloso, W. Meira and M. Zaki. ICDM 2006.
 * Demand-Driven Associative Classification. March, 2011. Book.
 * http://www.dcc.ufmg.br/~adrianov/THESIS/phd.pdf
 *
 *  Created on: Jul 18, 2012
 *      Author: itamar
 */

#include <algorithm>

#include "LacSupervised.h"
#include "DEclat.h"


/**
 * returns the accuracy
 */
float LacSupervised::predict(Trainning &trainning, char *ftest, AssociationRule*  associationRule){
	ui ntransactions = 0;
	ui nhits = 0;
	ui totalRules = 0;

	FILE* file=fopen(ftest,"r");
	if(file==NULL) {
		fprintf(stderr,"Test set %s not found.\n\n", ftest);
		exit(-1);
	}

	SymbolTable* featureTable = SymbolTable::FeaturesTable();
	SymbolTable* classesTable = SymbolTable::ClassesTable();

	vector<pair<string, ui> > classes;
	classesTable->getTableName(classes);

	unordered_set<ui> featureIds;
	const ui bufferSize = 200*1024;
	char line[bufferSize];

	ui tid, classId;

	ui nclasses = trainning.getNumberOfClasses();
	vector<float> scores(nclasses);
	ui defaultClass = trainning.getMostFrequentClass();

	while(fgets(line, bufferSize, file)){
		++ntransactions;
		classId = 10000000;
		processLineInput(line, featureTable, classesTable, tid, classId, featureIds);
		if(classId == 10000000){
			throw  string("Class Id not found: ") + line ;
		}

		Projection* projection = trainning.geProjection(featureIds);
		fill_n(scores.begin(), scores.size(), 0.0f);
		float totalScore = 0.0f;
		ui nrules = 0;
		ui prediction = defaultClass;
		AssociationRule::RulesResult result =  associationRule->induceRules(projection, minSupport, iminSupport, nclasses);
		for(ui classId = 0; classId < nclasses; ++classId){
			nrules += result.classesNRules[classId];
			float score = result.score(classId);
			totalScore += score;
			scores[classId] = score;
			if(cmp(scores[prediction], score) < 0){
				prediction = classId;
			}
		}
		totalRules += nrules;
	    nhits += prediction == classId;;
		printStatistics(trainning, tid, classesTable, classes, classId, prediction, scores, totalScore, nrules);

		delete projection;
	}

	fclose(file);

	//printf("%u\n", totalRules);

	return ntransactions == 0 ? 1.0 : static_cast<float>(nhits)/static_cast<float>(ntransactions);
}

void LacSupervised::printStatistics(Trainning &trainning, ui tid, SymbolTable* classesTable, vector<pair<string, ui> > &classes, ui classId,
									ui prediction, vector<float>& scores, float totalScore, ui nrules)
{
	ui correct = classId == prediction;
	printf("%f\n", scores[1]);


// 	//float ranking=0.f; //TODO fix raking
// 	float entropy=0.f;
// 
// 	//float i = 0.f;
// 	if(cmp(totalScore, 0.0f)){
// 		for (const pair<string, ui> &p : classes) {
// 			scores[p.second] /= totalScore;
// 			//ranking += i * scores[p.second];
// 			entropy -= scores[p.second] * log2(scores[p.second] + EPS);
// 			//i += 1.0f;
// 		}
// 	}else{
// 		for (const pair<string, ui> &p : classes) {
// 			scores[p.second] = trainning.getClassProb(p.second);
// 			//cerr << tid << " " << scores[p.second] << endl;
// 			//ranking += i * scores[p.second];
// 			entropy -= scores[p.second] * log2(scores[p.second] + EPS);
// 			//i += 1.0f;
// 		}
// 	}
// 	//printf("ranking= %.4f entropy= %.4f rules= %u",  ranking, entropy, nrules);
// 	printf("entropy= %.4f rules= %u", entropy, nrules);
// 
// 
// 	for (const pair<string, ui> &p : classes) {
// 		printf(" prob[%s]= %.4f", p.first.c_str(),scores[p.second]);
// 	}
// 	printf("\n");
}



