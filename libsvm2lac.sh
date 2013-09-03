#!/bin/bash

f_in=$1
f_ou=$2

sed "s/^0 /ID 0 w=/g" $f_in | sed "s/^1 /ID 1 w=/g" | sed "s/:1 / w=/g" | sed "s/:1//g" | sed "s/ID 0/99 CLASS=0/g" | sed "s/ID 1/99 CLASS=1/g"> $f_ou