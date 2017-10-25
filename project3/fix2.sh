#!/bin/bash

for threads in 1 2 4
do
	echo "called with theads:$threads..."
	g++ -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps project3.cpp -o prog -lm -fopenmp
                ./prog
done