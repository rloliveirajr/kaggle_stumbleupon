#!/bin/bash
rm -rf out4-0
path=~/Dropbox/machineLearning/tp2/data/
for file in $(ls $path)
do
    echo $file
    python Crossvalidation.py $path$file 4 0 >> out4-0
done

rm -rf out4-1
path=~/Dropbox/machineLearning/tp2/data/
for file in $(ls $path)
do
    echo $file
    python Crossvalidation.py $path$file 4 1 >> out4-1
done

