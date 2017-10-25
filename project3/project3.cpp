#include <omp.h>
#include <stdio.h>



struct s
{
	float value;
	//int pad[NUM];
} Array[4];

int main(int argc, char const *argv[])
{

	omp_set_num_threads( NUMT );

	unsigned int someBigNumber = 1000000000;	// if > 4B, use "long unsigned int"

	double start = omp_get_wtime( );
	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < someBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + 2.;
		}
	}
	double finish = omp_get_wtime( );
	double timing = (finish - start) * 1e6;
	printf("%f\t%f\n", timing, 4*someBigNumber/timing);

	return 0;
}