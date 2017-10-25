#include <stdio.h>
#include <stdlib.h> //
#include <omp.h>
#include "simd.p5.h"

int main(int argc, char const *argv[])
{
    int NUMS, NUMT;
    
    for (NUMS = 1024; NUMS <= 1024 * 1024 * 16; NUMS  *= 4)
    {
        float *A = (float *)malloc(sizeof(float)*NUMS);
        float *C = (float *)malloc(sizeof(float)*NUMS);
        float *B = (float *)malloc(sizeof(float)*NUMS);
        float sum[4] = { 0., 0., 0., 0};
        for (NUMT = 1; NUMT <= 16; NUMT *= 2)
        {
            double time_start = omp_get_wtime();  //time started
            SimdMul( A, B, C, NUMS);
       
            double time_end = omp_get_wtime();  //time ended

            double time_consumed = (time_end - time_start) * 1e6;
            printf("%.5lf\t", time_consumed, NUMS);
            printf("%.5lf\n", (long int)NUMS / time_consumed);
        }
    }
            return 0;
}