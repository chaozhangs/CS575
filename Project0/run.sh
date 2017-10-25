#!/bin/bash 

bodies=400
steps=300

for threads in 1 2 4 6 8
do
	echo "called with theads: $threads..."
	icpc -O3 -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps -o result_c_d.$threads project2_c_d.cpp -openmp
	icpc -O3 -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps -o result_c_s.$threads project2_c_s.cpp -openmp
	icpc -O3 -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps -o result_f_d.$threads project2_f_d.cpp -openmp
	icpc -O3 -DNUMT=$threads -DNUMBODIES=$bodies -DNUMSTEPS=$steps -o result_f_s.$threads project2_f_s.cpp -openmp
	# icpc -O3 -DNUMS=$nums -DNUMT=$threads -o result project1.cpp -openmp
	# ./result >> out
done
