#include <stdio.h>
#include <time.h>
/*DOES NOT ACCOUNT FOR ERRORS
* Simple time interval
* Will output the time to execute according to CPU time
* MUST COMPILE USING "gcc -lrt -o time time.cpp"
* the -lrt as far as I can tell is for the "librt" library
* For my module, I get a pretty stable 3 micro seconds per run
* but that's not running anything aside from this...
*/
int main(){
	struct timespec timestart, timeend;
	for(int i = 0; i<200; i++){
		clock_gettime(CLOCK_REALTIME, &timestart);
		clock_gettime(CLOCK_REALTIME, &timeend);
		printf("%lu nanoseconds\n", timeend.tv_nsec-timestart.tv_nsec);
	}
}
