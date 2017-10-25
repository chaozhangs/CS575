#!/bin/csh
#number of threads:
foreach t ( 1 4 8 16 32)
        foreach s (512 1024 1536 2048 3072 4096)
                g++ -DNUMS=$s -DNUMT=$t project1.cpp -o prog -lm -fopenmp
                ./prog
        end
end
