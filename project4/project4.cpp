#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define Ranf(a,b) ((float)rand()/(float)(RAND_MAX/(b-a))+a)

int		NowYear;		// 2016 - 2021
int		NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int		NowNumDeer;		// number of deer in the current population
float	NowLightDur;	// number of light duration per month
int 	NowNumHunter;

const float GRAIN_GROWS_PER_MONTH   = 8.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;

const float AVG_PRECIP_PER_MONTH    = 6.0;
const float AMP_PRECIP_PER_MONTH    = 6.0;
const float RANDOM_PRECIP           = 2.0;

const float AVG_TEMP                = 50.0;
const float AMP_TEMP                = 20.0;
const float RANDOM_TEMP             = 10.0;

const float MIDTEMP                 = 40.0;
const float MIDPRECIP               = 10.0;
const float MIDLIGHTDUR				= 100.;

const float RANDOM_OVERCAST			= 8.0;

void DeerHunter() {
	while(NowYear <= 2021) {
		int tempNumHunter = NowNumHunter;
		int tempNumDeer = NowNumDeer;
		float huntFactor = 1.;
		if (tempNumDeer == tempNumHunter)
			huntFactor = (float)tempNumDeer * (float)tempNumHunter / (float)tempNumHunter;
		else
			huntFactor = (float)tempNumHunter * (float)tempNumDeer / (float)abs(tempNumDeer - tempNumHunter);
		int huntNum = (int)(Ranf(0., huntFactor));


		if (tempNumHunter == 0 && tempNumDeer >= 3) 
			tempNumHunter = 1;
		else if (tempNumHunter > 0 && tempNumDeer >= 3) {
			if (huntNum >= 1) {
				tempNumHunter ++;
				tempNumDeer -= huntNum;
			}
			else {
				tempNumHunter --;
			}
		}
		else if (tempNumHunter != 0 && tempNumDeer < 3)
			tempNumHunter --;
		#pragma omp barrier

		if (tempNumHunter < 0)
			tempNumHunter = 0;
		NowNumHunter = tempNumHunter;
		#pragma omp barrier
		#pragma omp barrier
	}
}

void GrainDeer() {
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		int tempNumDeer;
		float huntFactor = 1.;
		int huntNum;
		if (NowNumDeer == NowNumHunter)
			huntFactor = (float)NowNumDeer * (float)NowNumHunter / (float)NowNumHunter;
		else
			huntFactor = (float)NowNumHunter * (float)NowNumDeer / (float)abs(NowNumDeer - NowNumHunter);
		huntNum = (int)(Ranf(0., huntFactor));
		
		if( NowNumDeer < NowHeight )
			tempNumDeer = NowNumDeer + 1;
		else
			tempNumDeer = NowNumDeer - 1;
		
		if (NowNumHunter > 0 && NowNumDeer >= 3 && huntNum >= 1) {
			tempNumDeer -= huntNum;
		}

		if(tempNumDeer < 0)
			tempNumDeer = 0;

		// DoneComputing barrier:
		#pragma omp barrier

		NowNumDeer = tempNumDeer;
		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier

	}
}

void Grain() {
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		float tempFactor, precipFactor, lightFactor, tempHeight;
		tempFactor = expf(-(powf((( NowTemp - MIDTEMP )/10.0), 2.)));
		precipFactor = expf(-(powf((( NowPrecip - MIDPRECIP )/10.0), 2.)));
		lightFactor = expf(-(powf((( NowLightDur - MIDLIGHTDUR )/10.0), 2.)));

		tempHeight = NowHeight + tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		if (tempHeight < 0.) tempHeight = 0.;
		// DoneComputing barrier:
		#pragma omp barrier

 		NowHeight = tempHeight;
		// DoneAssigning barrier:
		#pragma omp barrier


		// DonePrinting barrier:
		#pragma omp barrier

	}
}

void Watcher() {
	while( NowYear <= 2021 )
	{
		// compute a temporary next-value for this quantity based on the current state of the simulation:
		

		// DoneComputing barrier:
		#pragma omp barrier


		// DoneAssigning barrier:
		#pragma omp barrier


		if (NowMonth < 9)
			printf("%d0%d\t", NowYear, NowMonth+1);
		else
			printf("%d%d\t", NowYear, NowMonth+1);
		printf("%f\t%f\t", NowTemp, NowPrecip);
		printf("%f\t%d\t%d\n", NowHeight, NowNumDeer, NowNumHunter);

		NowMonth++;
		if (NowMonth == 12) {
			NowMonth = 0;
			NowYear++;
		}
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
		if( NowPrecip < 0. )
			NowPrecip = 0.;

		#pragma omp barrier

	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	// The light duration (hours per month)
	float lightDur = 8. * sin(M_PI * (float)NowMonth / 11.) + 8.;
	NowLightDur = 0.;
	for (int i = 0; i < 30; i++) {
		NowLightDur += lightDur - Ranf(0., RANDOM_OVERCAST);
	}
	
	// starting date and time:
	NowMonth =    0;
	NowYear  = 2016;

	// starting state (feel free to change this if you want):
	NowNumDeer = 1;
	NowHeight  = 1.;
	NowNumHunter = 1;

	omp_set_num_threads( 4 );
	printf("Data\tTemperature\tPrecipitation\tGrain\tDeer\tHunter\n");
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}
		
		#pragma omp section
		{
			DeerHunter( );
		}
	}       // implied barrier -- all functions must return to get past here
	return 0;
}

