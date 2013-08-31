from sklearn.ensemble import RandomForestRegressor
from sklearn.naive_bayes import MultinomialNB
from sklearn.ensemble import AdaBoostClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.naive_bayes import BernoulliNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.datasets import load_svmlight_file
from sklearn.datasets import load_svmlight_files
from sklearn import cross_validation
from sklearn.metrics import roc_auc_score
import numpy as np
import argparse


def cross_validate(clf, X, y):
  print "Calc url weigth"
  #weigth_u = cross_validation.cross_val_score(clf, X["url"].toarray(), y, cv=3).mean()
  weigth_u = 1.0
  print "Calc body weigth"
  #weigth_b = cross_validation.cross_val_score(clf, X["body"].toarray(), y, cv=3).mean()
  weigth_b = 1.0
  print "Calc title weigth"
  #weigth_t = cross_validation.cross_val_score(clf, X["title"].toarray(), y, cv=3).mean()
  weigth_t = 1.0
  print "Calc title all"
  #weigth_a = cross_validation.cross_val_score(clf, X["all"].toarray(), y, cv=3).mean()
  weigth_a = 1.0
  
  ss = cross_validation.ShuffleSplit(len(y), n_iter=5, random_state=0)
  i = 1
  for train_index, test_index in ss:
    print "Iteration: %d" % i
    i+=1
    train_b = X["body"][train_index]
    train_u = X["url"][train_index]
    train_t = X["title"][train_index]
    train_a = X["all"][train_index]
    
    x_labels = y[train_index]
    
    test_b = X["body"][test_index]
    test_u = X["url"][test_index]
    test_t = X["title"][test_index]
    test_a = X["all"][test_index]
    
    t_labels = y[test_index]
    
    clf.fit(train_b.toarray(),x_labels)
    probs_b = clf.predict_proba(test_b.toarray())
    
    clf.fit(train_u.toarray(),x_labels)
    probs_u = clf.predict_proba(test_u.toarray())
    
    clf.fit(train_t.toarray(), x_labels)
    probs_t = clf.predict_proba(test_t.toarray())
    
    clf.fit(train_a.toarray(), x_labels)
    probs_a = clf.predict_proba(test_a.toarray())
    
    probs = []
    for i in range(len(test_index)):
      score_i = (probs_b[i,1]*weigth_b + probs_u[i,1]*weigth_u + probs_t[i,1]*weigth_t + probs_a[i,1]*weigth_a)/4.0
      probs.append(score_i)
      
    scores = roc_auc_score(t_labels, probs)
    
    print scores
  return scores

def predict(clf, X, y, T, t):
  
  print "Calc url weigth"
  weigth_u = cross_validation.cross_val_score(clf, X["url"].toarray(), y, cv=3).mean()
  print "Calc body weigth"
  weigth_b = cross_validation.cross_val_score(clf, X["body"].toarray(), y, cv=3).mean()
  print "Calc title weigth"
  weigth_t = cross_validation.cross_val_score(clf, X["title"].toarray(), y, cv=3).mean()
  #print "Calc all weigth"
  #weigth_a = cross_validation.cross_val_score(clf, X["all"].toarray(), y, cv=3).mean()
  
  train_b = X["body"]
  train_u = X["url"]
  train_t = X["title"]
  #train_a = X["all"]
  
  x_labels = y
  
  test_b = T["body"]
  test_u = T["url"]
  test_t = T["title"]
  #test_a = T["all"]
  
  clf.fit(train_b.toarray(),x_labels)
  probs_b = clf.predict_proba(test_b.toarray())
  
  clf.fit(train_u.toarray(),x_labels)
  probs_u = clf.predict_proba(test_u.toarray())
  
  clf.fit(train_t.toarray(), x_labels)
  probs_t = clf.predict_proba(test_t.toarray())
  
  #clf.fit(train_a, x_labels)
  #probs_a = clf.predict_proba(test_a)
  
  probs = []
  for i in range(len(t)):
    score_i = (probs_b[i,1]*weigth_b + probs_u[i,1]*weigth_u + probs_t[i,1]*weigth_t)/3.0
    probs.append(score_i)
      
  return probs
  
parser = argparse.ArgumentParser()
#parser.add_argument( "train_file" )
parser.add_argument( "-p", "--predict", help = "if is to make predictions in a test file", default = None )
parser.add_argument( "-t", "--predict_file", help = "if is to make predictions in a test file", default = None )
parser.add_argument( "-c", "--cross_validation", help = "if have make cross-validation", default = None )

args = parser.parse_args()

classifier = RandomForestRegressor(n_estimators=1000,verbose=2,n_jobs=5,min_samples_split=5,random_state=1034324)
#classifier = RandomForestClassifier()

X_url, y, X_title, y_t, X_body, y_b, X_a, y_a = load_svmlight_files(("url_train.txt", "title_train.txt", "body_train.txt", "all_train.txt"))
X = {"url":X_url, "title": X_title, "body": X_body, "all": X_a}

if(args.predict):
  print "Predicting"
  T_url, t, T_title, y_t, T_body, y_b, T_a, y_a = load_svmlight_files(("url_test.txt", "title_test.txt", "body_test.txt", "all_test.txt"))
  T = {"url": T_url, "title": T_title, "body": T_body, "all": T_a}
  probs = predict(classifier, X, y, T, t)
  
  f = open("sub_31-08_01h15.txt","w")
  f.write("label\n")
  for p in probs:
    line = "%f\n" % p
    f.write(line)
  f.close()
elif(args.cross_validation):
  
  cross_validate(classifier, X, y)