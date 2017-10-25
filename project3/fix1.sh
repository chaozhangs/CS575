#!/bin/bash 

for padnum in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
do
	for threads in 1 2 4
	do
		echo "called with theads:$threads and padnums:$padnum"
		g++ -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps project3.cpp -o prog -lm -fopenmp
                ./prog
	done
done
