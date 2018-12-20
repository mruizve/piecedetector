#include<limits.h>
#include<string.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include<unistd.h>
#include<sys/syscall.h>
#include<bitset>
#include<iostream>
#include "flexibowl.h"
#include "utils.h"

static void interface_cleanup(void *_arg)
{
	// get timer
	timer_t* timer = (timer_t*)_arg;

	// destroy timer
	if( 0 > timer_delete(*timer) )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mtimer_delete\e[0;39m(): ") << strerror(errno) << std::endl;
	}
}

// initialize and launch the kinematics abstraction thread
void* smf::robots::Flexibowl::interface_helper(void* _robot)
{
	int err;
	timer_t timer;

	// get robot
	smf::robots::Flexibowl* robot = (smf::robots::Flexibowl*)_robot;

	// disable cancellation before timer setup
	err = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_setcancelstate\e[0;39m(): ") << strerror(err) << std::endl;
		return NULL;
	}

	// add the cancellation clean-up header
	pthread_cleanup_push(&interface_cleanup, &timer);

	// prepare signal event
	struct sigevent sigevent;
	(void)memset(&sigevent, 0, sizeof(struct sigevent));
	sigevent.sigev_signo = smf::robots::SIGNAL();
	sigevent.sigev_value.sival_ptr = NULL;
	sigevent.sigev_notify = SIGEV_THREAD_ID;
	sigevent._sigev_un._tid = syscall(SYS_gettid);

	// create internal POSIX timer
	err = timer_create(CLOCK_REALTIME, &sigevent, &timer);
	if( 0 > err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mtimer_create\e[0;39m(): ") << strerror(errno) << std::endl;
		return NULL;
	}

	// extract timer expiration time seconds and nanoseconds
	int sec = robot->interval_ / 1000;
	int nsec = 1000000 * (robot->interval_ % 1000);

	// start timer
	struct itimerspec timeout =
	{
		.it_interval = {sec, nsec},	.it_value = {sec, nsec}
	};
	err = timer_settime(timer, 0, &timeout, NULL);
	if( 0 > err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mtimer_settime\e[0;39m(): ") << strerror(errno) << std::endl;
		return NULL;
	}

	// enable cancellation after timer setup
	err = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_setcancelstate\e[0;39m(): ") << strerror(err) << std::endl;
		return NULL;
	}

	robot->command("servo=1");
	robot->command("angle=1");
	robot->command("speed=10");
	robot->command("acc=100");
	robot->command("dec=100");
	robot->command("b_angle=1");
	robot->command("b_speed=10");
	robot->command("b_acc=100");
	robot->command("b_dec=100");

	// launch interface
	robot->interface();

	// remove the cancellation clean-up header
	pthread_cleanup_pop(0);
}

void smf::robots::Flexibowl::interface(void)
{
	bool direction = true;
	int signo;
	siginfo_t info;

	do
	{
		// wait until the timer expires
		signo = sigwaitinfo(&sigset_, &info);
		if( 0 > signo )
		{
			std::cerr << SMF_WHAT("unexpected error from \e[1;31msigwait_info\e[0;39m(): ") << strerror(errno) << std::endl;
		}

		int status;
		try
		{
			status = this->command("sr");
		}
		catch( std::exception &e )
		{
			std::cerr << SMF_WHAT("cannot send SR command to the flexibowl\n>> ") + e.what() << std::endl;
		}
		
		if( FLEXIBOWL_STATUS_READY & status )
		{
			if( direction )
			{
				this->command("forward=1");
			}
			else
			{
				this->command("backward=1");
			}

			direction = (rand() % 1000 > 990)? !direction : direction;
		}
	}
	while( 0 < signo );
}
