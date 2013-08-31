#!/bin/python

import csv
import json

def comp(a,b):
  EPS = 1E-15
  
  if(a > b - EPS):
    if(b > a - EPS):
      return 0;
    else:
      return 1;
  return -1

score_url = 0.73344
score_tite = 0.75710
score_body = 0.77918
score_all = 0.79811

i_f = open( "p_2.csv" )
reader = csv.reader( i_f, delimiter = "," )
reader.next()

output = open("sub.txt", "w")
output.write("urlid,label\n")

for line in reader:
  url_id = line.pop(0)
  title = line.pop(0)
  url = line.pop(0)
  body = line.pop(0)
  al = line.pop(0)
  
  score = (score_url*float(url) + score_tite*float(title) + score_body*float(body) + score_all * float(al))/4.0
  l = "%s,%s\n" % (url_id, score)
  #if(comp(score,0.5) > 0):
    #l = "%s,%s\n" % (url_id, 1)
  #else:
    #l = "%s,%s\n" % (url_id, 0)
  output.write(l)
  
output.close()