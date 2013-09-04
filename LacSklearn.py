import numpy as np
import os
from sklearn.base import BaseEstimator
import random

class Lac(BaseEstimator):
    def __init__(self, ruleSize=3):
        self.ruleSize=3
        self.trainData = None
        self.trainTarget = None
            
    def fit(self, X, Y):
        self.trainData = X
        self.trainTarget = Y
        
    def predict(self, X):
#        print X
        randomName = 'lac_' + str(random.randint(0, 1000000000)) 
        targetTest = [0 for i in range(len(X))]
        (strain, stest) = self.__createTraingTest(X, targetTest, randomName)
        r = self.__runLac(strain, stest, randomName)
        os.remove(strain)
        os.remove(stest)
        #print r
        return r
    
    def score(self, X, y):
        """Returns the mean accuracy on the given test data and labels.
        
        Parameters
        ----------
        X : array-like, shape = [n_samples, n_features]
           Training set.
        
        y : array-like, shape = [n_samples]
           Labels for X.
        
        Returns
        -------
        z : float
        
        """
    
       
        Y = self.predict(X)
#        print 'Meu score'
#        print y
#        print np.array(Y)
        m= np.mean(Y == y)
#        print m
        return m
    
    def __runLac(self, strain, stest, randomName, cache=0, support=1, confidence=0.001, supervised=0):
        outputName = randomName + '.out';
        cmd = str.format("./lazy -i {0} -t {1} -e {2} -s {3} -c {4} -m {5} -d {6} > {7}", 
                                strain, stest, cache, support, confidence, self.ruleSize, supervised, outputName);
    
        #print("running:", cmd);              
        os.system(cmd);
        return self.__processResult(outputName);
    
    def __processResult(self, inputName):
        predict = []
        inputFile = open(inputName, 'r');
        lines = inputFile.readlines();
        inputFile.close()
        os.remove(inputName)
        total = 0.0
        acc = 0.0
        
        start = 3;
        end = len(lines)# - 21;#21 is the size of tail statics
        
        for i in range(start, end, 2):
            if lines[i][0] == 'C':
                break
            fields = lines[i].split();
            label = int(fields[5]);
            predicted = int(fields[9]);
            predict.append(predicted)
                    
        return predict

    def __createTraingTest(self, testData, testTarget, randomName):
        stest = randomName + '.test'
        strain = randomName + '.train'
        self.__writeFile(strain, self.trainData, self.trainTarget, 0)
        self.__writeFile(stest, testData, testTarget, 1000)
        
        return (strain, stest)
        
    def __writeFile(self, path, d, t, shift):
        f = open(path, 'w')
        for i  in range(len(d)):
            f.write(str.format('{0} CLASS={1}', i + shift, t[i])),
            for col in range(len(d[i])):
                f.write(str.format(' w[{0}]={1}', col+1,  d[i][col])),
            f.write('\n')

        f.close()   
