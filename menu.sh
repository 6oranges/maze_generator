#!/bin/bash

for i in {1..1}
do 
    ./test -d 100,100 -p 10 -b 5 -c -e 255,255,255 -w 0,0,0 -f "hard$i.ppm" -s 255,0,0 -f "hardSolved$i.ppm"
done
for i in {1..4}
do 
    ./test -d 50,50 -p 10 -b 5 -c -e 255,255,255 -w 0,0,0 -f "medium$i.ppm" -s 255,0,0 -f "mediumSolved$i.ppm"
done
for i in {1..16}
do 
    ./test -d 25,25 -p 10 -b 5 -c -e 255,255,255 -w 0,0,0 -f "easy$i.ppm" -s 255,0,0 -f "easySolved$i.ppm"
done
