#!/bin/python

import csv
import json

with open('train.tsv', 'rb') as csvfile:
  sample = csv.reader(csvfile, delimiter='\t')
  i=1
  for row in sample:
    if i == 1:
      i += 1
      continue
    text = row[2]
    text = text.replace("{\"","").replace("\",\""," ").replace("\"}","").replace("\":\""," ").replace(" ", " w=")
    instance = "%s CLASS=%s w=%s" % (row[1],row[-1], text)
    print instance