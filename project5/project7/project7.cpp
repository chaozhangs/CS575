#include <stdio.h>
#include <stdlib.h> //
#include <omp.h>
#include "simd.p5.h"

int main(int argc, char const *argv[])
{
    int ARRAYS;
    
    for (ARRAYS = 1024; ARRAYS <= 1024 * 1024 * 16; ARRAYS  *= 4)
    {
        float *A = (float *)malloc(sizeof(float)*ARRAYS);
        float *C = (float *)malloc(sizeof(float)*ARRAYS);
        float *B = (float *)malloc(sizeof(float)*ARRAYS);
        
       
        double time_start = omp_get_wtime();  //time started

    
        for( long int i = 0; i < ARRAYS; i++ )
        {
            C[i] = A[i]*B[i];
        }
            
        double time_end = omp_get_wtime();  //time ended

        double time_consumed = (time_end - time_start) * 1e6;
        printf("%.5lf\t", time_consumed);
        printf("%.5lf\n", (long int)ARRAYS / time_consumed);
    }
            return 0;
}