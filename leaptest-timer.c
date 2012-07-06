/* Leap second timer test
 *              by: john stultz (johnstul@us.ibm.com)
 *              (C) Copyright IBM 2012
 *              Trimmed down by Geoffrey Thomas <gthomas@mokafive.com>
 *              (C) Copyright 2012  moka5, Inc.
 *              Licensed under the GPL
 */


#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>


#define CALLS_PER_LOOP 64
#define NSEC_PER_SEC 1000000000ULL

struct timespec timespec_add(struct timespec ts, unsigned long long ns)
{
	ts.tv_nsec += ns;
	while(ts.tv_nsec >= NSEC_PER_SEC) {
		ts.tv_nsec -= NSEC_PER_SEC;
		ts.tv_sec++;
	}
	return ts;
}

struct timespec timespec_diff(struct timespec a, struct timespec b)
{
	long long ns;
	int neg = 0;

	ns = a.tv_sec *NSEC_PER_SEC + a.tv_nsec;
	ns -= b.tv_sec *NSEC_PER_SEC + b.tv_nsec;

	if (ns < 0) {
		neg = 1;
		ns = -ns;
	}
	a.tv_sec = ns/NSEC_PER_SEC;
	a.tv_nsec = ns%NSEC_PER_SEC;

	if (neg) {
		a.tv_sec = -a.tv_sec;
		a.tv_nsec = -a.tv_nsec;
	}

	return a;
}


int  main(void)
{
	long now, then;
	struct timespec ts;

	int clock_type 		= CLOCK_REALTIME;
	int flag 		= TIMER_ABSTIME;
	long long sleeptime	= NSEC_PER_SEC/2;

	clock_gettime(clock_type, &ts);
	now = then = ts.tv_sec;
	while(now - then < 10){
		struct timespec target, diff, rem;
		rem.tv_sec = 0;
		rem.tv_nsec = 0;

		target = timespec_add(ts, sleeptime);

		clock_nanosleep(clock_type, TIMER_ABSTIME, &target, &rem);
		clock_gettime(clock_type, &ts);

		diff = timespec_diff(ts, target);
		printf("diff: %Lf\n", diff.tv_sec + ((long double)0.000000001 * diff.tv_nsec));
		now = ts.tv_sec;
	}

	return 0;
}
