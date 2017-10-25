#!/bin/bash
bodies=400
steps=200

for threads in 1 2 4 6 8
do
echo "called with theads: $threads..."
                g++ -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps project2_f_d.cpp -o prog -lm -fopenmp
                ./prog
done