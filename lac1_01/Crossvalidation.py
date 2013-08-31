import random
import os
import math
import sys

def calcMean(a):
    return sum(a)/float(len(a));

def calcStd(a, m):
    q =  map(lambda x: (x-m)*(x-m), a);
    return math.sqrt(sum(q)/( len(a) - 1));

class Crossvalidation:
    def __init__(self, path, ruleSize=3, ruleMode=0, partions=5):
        f = open(path, 'r')
        self.lines = f.readlines()
        self.ruleSize = ruleSize
        self.ruleMode = ruleMode
        self.filename = path.split('/')[-1]
        self.size = len(self.lines)
        self.__shufle();
        self.partions = partions
        self.ranges = self.__genRanges()
        f.close()
    
    def run(self):
        ret =  {'time':[], 'memory':[], 'accuracy':[], 'nrules':[]}
        for i in range(self.partions):
            trainTest = self.printTrainningTest(i)
            t = self.__runLac(trainTest[0], trainTest[1])
            for k in t:
                ret[k].append(t[k])
        self.__printStatistics(ret)
        return ret
    
    def __printStatistics(self, results):
        print self.filename,
        for measure in results:
            mean = calcMean(results[measure])
            std = calcStd(results[measure], mean)
            print measure, mean, std,
        
        print
    
    def __runLac(self, strain, stest,  cache=500000, support=1, confidence=0.001, supervised=0):
        outputName =  'acc.out.txt';
        cmd = str.format(r'/usr/bin/time -f "%M\n%U" -o time.out ./lazy -i {0} -t {1} -e {2} -s {3} -c {4} -m {5} -d {6} -a {7}  > {8}', 
                                strain, stest, cache, support, confidence, self.ruleSize, supervised, self.ruleMode, outputName);
        mode = ' ECLAT'
        if self.ruleMode == 1:
             mode = ' DIFF ECLAT'
        sys.stderr.write(cmd + mode + "\n");
                          
        os.system(cmd);
        result = {'time':0., 'memory':0., 'accuracy':0., 'nrules':0.}
        #with open(outputName, 'r') as fresult:
         #   result['nrules'] = float(fresult.readline())
          #  result['accuracy'] = float(fresult.readline())
        with open('time.out', 'r') as fresult:
            result['memory'] = float(fresult.readline())/1024.0
            result['time'] = float(fresult.readline())
        
        sys.stderr.write(str(result)+ "\n");
        return result
     
    def __shufle(self, seed=1):
        random.seed(seed)
        for i in range(self.size - 1):
            j = random.randint(i+1, self.size - 1)
            tmp = self.lines[i]
            self.lines[i] = self.lines[j]
            self.lines[j] = tmp

    def __genRanges(self):
        ret = []
        step = self.size/self.partions;
        
        t =  self.size % self.partions;
        a = [ step+1 if i < t else step for i in range(self.partions)]
        
        start = 0
        end = 0
    
        for i in range(self.partions):
            end += a[i]
            ret.append((start, end-1))
            start = end
        
        return ret;

    def printTrainningTest(self, turn):
        trainning = open('trainning.txt', 'w')
        test = open('test.txt', 'w')
        
        for i in range(self.size):
            if i < self.ranges[turn][0] or i > self.ranges[turn][1]:
                trainning.write(self.lines[i])
            else:
                test.write(self.lines[i])
        
        ret = (trainning.name, test.name)
        trainning.close()
        test.close()
        return ret
                
                
path = sys.argv[1] #'/home/itamar/workspacePython/activeLearning/tmp.txt'
ruleSize = int(sys.argv[2])
ruleMode = int(sys.argv[3])

#path='/home/itamar/workspacePython/activeLearning/tmp.txt'
#ruleSize=3
cross = Crossvalidation(path, ruleSize, ruleMode)
cross.run()

    
