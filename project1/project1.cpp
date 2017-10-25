#include <omp.h>
#include <stdio.h>

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.
float
Height( int iu, int iv )	// iu,iv = 0 .. NUMS-1
{
	float u = (float)iu / (float)(NUMS-1);
	float v = (float)iv / (float)(NUMS-1);

	// the basis functions:

	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	// finally, we get to compute something:

	float height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
			+ bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
			+ bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
			+ bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

	return height;
}


int main( int argc, char *argv[ ] )
{

	float volume = 0.;
	// the area of a single full-sized tile:
	float fullTileArea = (  ( (XMAX-XMIN)/(float)(NUMS-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	omp_set_num_threads(NUMT);

	double start = omp_get_wtime();

	#pragma omp parallel for collapse(2) reduction(+:volume)
	for( int iv = 0; iv < NUMS; iv++ )
	{
		for( int iu = 0; iu < NUMS; iu++ )
		{
			float h = Height(iu, iv);
			float area = h * fullTileArea;

			if (iv == 0 || iv == NUMS-1){
				if (iu == 0 || iu == NUMS-1){
					area /= 4;
				}else{
					area /= 2;
				}
			}else{
				if (iu == 0 || iu == NUMS-1){
					area /= 2;
				}
			}

			volume += area;

		}
	}

	double end = omp_get_wtime();
        double sum_height_calculated = (double)(NUMS * NUMS)/(end-start)/1000000.;


	printf("Number of threads:      %d\n", NUMT);
	printf("Number of subdivisions: %d\n", NUMS);
	printf("Total Area:             %f\n", volume);
	printf("Average Performance:    %.2f MegaHeights Computed/Sec\n", sum_height_calculated);
	
}
