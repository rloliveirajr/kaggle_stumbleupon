
#include <getopt.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unordered_map>

#include "Trainning.h"
#include "LacSupervised.h"
#include "DEclat.h"
#include "Eclat.h"
#include "timer.h"

enum class Mode {
	LAZY_SUPERVISED_CLASSIFICATION=0, LAZY_ACTIVE_CLASSIFICATION=3
};

enum class AssociationRuleMode {
	ECLAT=0, DECLAT=1
};

using namespace std;
unordered_map<string, unsigned long long> PROOF;

float classify(Mode mode, AssociationRuleMode ruleMode, char* file_in, char* file_test, float minConfindence, float minSupport, ui iminSupport,  ui maxRuleSize, ui maxCacheSize);

int main(int argc, char** argv)
{
	Mode mode=Mode::LAZY_SUPERVISED_CLASSIFICATION;
	bool ruleModeSet = false;
	AssociationRuleMode ruleMode=AssociationRuleMode::ECLAT;
	char *file_in=NULL, *file_test=NULL;
	int c;
	float minConfidence = 0.001f, minSupport = 0.f;
	ui iminSupport = 1;
	ui maxRuleSize = 1, maxCacheSize=100000;

	const char* help ="i: trainning file path\n"
					  "t: test file path\n"
					  "c: min confidence\n"
					  "g: min relative support (percent)\n"
					  "s: min absolute support (integer)\n"
					  "d: mode (0 - supervised, 3 - active (not implemented yet))\n"
					  "m: max rule length\n"
					  "e: cache size\n"
					  "a: association rule mode (0 - Eclat, 1 - DEclat)\n"
					  "h: help\n";

	while((c=getopt(argc,argv,"i:t:c:e:g:s:d:m:a:h"))!=-1) {
			switch(c) {
				case 'i': file_in=strdup(optarg);
					  break;
				case 't': file_test=strdup(optarg);
					  break;
				case 'c': minConfidence=atof(optarg);
					  break;
				case 'g': minSupport=atof(optarg);
					iminSupport = 0;
					  break;
				case 's': iminSupport=atoi(optarg);
					  break;
				case 'e': maxCacheSize=atoi(optarg);
					  break;
				case 'd': mode= static_cast<Mode>(atoi(optarg));
					  break;
				case 'm': maxRuleSize=atoi(optarg);
					  if(!ruleModeSet){
						  ruleMode = maxRuleSize < 4 ? AssociationRuleMode::ECLAT : AssociationRuleMode::DECLAT;
					  }
					  break;
				case 'a': ruleMode=static_cast<AssociationRuleMode>(atoi(optarg));
						ruleModeSet = true;
						break;
				case 'h':
					printf("%s",help);
					exit(0);
					break;
				default:  exit(1);
			}
	}

	if(!file_in){
		fprintf(stderr, "trainning file not found!\n");
		fprintf(stderr, "%s", help);
		exit(2);
	}
	if(!file_test){
		fprintf(stderr, "test file not found!\n");
		fprintf(stderr, "%s", help);
		exit(3);
	}

	float accuracy = classify(mode, ruleMode, file_in, file_test, minConfidence, minSupport, iminSupport, maxRuleSize, maxCacheSize);

// 	printf("ACCURACY %.4f\n", accuracy);
// 	printTime();

	return 0;
}


float classify(Mode mode, AssociationRuleMode ruleMode, char* file_in, char* file_test, float minConfindence, float minSupport, ui iminSupport, ui maxRuleSize, ui maxCacheSize)
{
	__START_TIMER__
	float accuracy = 0.0f;
	if(Mode::LAZY_SUPERVISED_CLASSIFICATION == mode){
		Trainning trainning(maxCacheSize);
		trainning.readInput(file_in);
		free(file_in);
		LacSupervised lac(minConfindence, minSupport, iminSupport, maxRuleSize, maxCacheSize);
		AssociationRule*  associationRule = NULL;
		if(ruleMode == AssociationRuleMode::ECLAT){
		  associationRule = new Eclat(minConfindence, maxRuleSize, trainning.getTidClassMap(), maxCacheSize);
		}else if(ruleMode == AssociationRuleMode::DECLAT){
			 associationRule = new DEclat(minConfindence, maxRuleSize, trainning.getTidClassMap(), maxCacheSize);
		}
		accuracy = lac.predict(trainning, file_test, associationRule);
		delete associationRule;
		free(file_test);
	}else{
		throw "Mode not implemented";
	}

	__FINISH_TIMER__
	return accuracy;
}


