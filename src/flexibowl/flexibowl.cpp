#include<string.h>
#include<iostream>
#include<sstream>
#include "flexibowl.h"
#include "utils.h"

// flexibowl initialization with collaborative IP discovery, if required
smf::robots::Flexibowl::Flexibowl(const std::string _ip, const std::string _port, int _interval)
try :
	UDP(_ip, _port)
{
	// validate control loop interval
	if( 0 >= _interval )
	{
		throw SMF_ERROR("\e[1;31minvalid\e[0;39m control loop interval");
	}

	// set control loop interval
	interval_ = _interval;

	// if the IP of the flexibowl was not defined, then
	if( !ip_.compare(smf::net::BROADCAST_DISCOVERY()) )
	{
		FlexibowlMagic magic;

		// determine the IP by broadcast
		try
		{
			ip_ = smf::net::discoverIP("255.255.255.255", "5657", &magic, 5);
		}
		catch( std::exception &e )
		{
			throw SMF_ERROR("network address discovery \e[1;31mfailure\e[0;39m\n>> " + std::string(e.what()));
		}
	}

	// show destination IP address
	std::cout << SMF_WHAT("Flexibowl IP address: " + ip_) << std::endl;

	// verify the flexibowl network reachability
	try
	{
		for( bool wait = true; wait; )
		{
			int status;
			std::stringstream ss;

			try
			{
				status = smf::utils::exec("/bin/ping", "-c1", "-W1", ip_.c_str(), NULL);
			}
			catch( std::exception &e )
			{
				throw SMF_ERROR("\e[1;31munexpected\e[0;39m error during /bin/ping execution\n>> " + std::string(e.what()));
			}

			switch( status )
			{
				case 0: // success code
					// flexibowl found
					std::cout << SMF_WHAT("Flexibowl status: \e[1;32monline\e[0;39m") << std::endl;
					wait = false;
					break;

				case 1: // no reply code
					// flexibowl not found
					std::cout << SMF_WHAT("Flexibowl status: \e[1;34moffline\e[0;39m, waiting...") << std::endl;
					break;

				default:
					//  2: other errors
					// -1: died by exception
					ss << "unexpected /bin/ping exit status (code: " << status << ")";
					throw SMF_ERROR(ss.str());
			}
		}
	}
	catch(std::exception &e)
	{
		std::cerr << SMF_WHAT("Flexibowl status: \e[1;33munknown\e[0;39m\n>> ") << e.what() << std::endl;
	}

	// start the UDP communication with the Flexibowl server
	try
	{
		this->start();
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("cannot communicate with the Flexibowl server\n>> " + std::string(e.what()));
	}

	// initialize mutex
	int err = pthread_mutex_init(&mutex_, NULL);
	if( 0 != err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mpthread_mutex_init\e[0;39m(): " + std::string(strerror(err)));
	}

	// prepare signal set to be ignored by all threads
	// (block SIGRTMIN in the main thread, other threads created from
	// this thread will inherit a copy of the signal mask)
	(void)sigemptyset(&sigset_);
	(void)sigaddset(&sigset_, smf::robots::SIGNAL());
	err = pthread_sigmask(SIG_BLOCK, &sigset_, NULL);
	if( 0 != err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mpthread_sigmask\e[0;39m(): "+ std::string(strerror(err)));
	}

	// create the abstraction thread
	err = pthread_create(&thread_, NULL, &smf::robots::Flexibowl::interface_helper, this);
	if( 0 != err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mpthread_create\e[0;39m(): " + std::string(strerror(err)));
	}
}
catch( std::exception &e )
{
	throw SMF_ERROR("caught exception during Flexibowl instantiation\n>> " + std::string(e.what()));
}

// stop the interface thread and close the communication channel
smf::robots::Flexibowl::~Flexibowl(void)
{
	// stop the interface thread
	int err = pthread_cancel(thread_);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_cancel\e[0;39m(): ") << strerror(err) << std::endl;
	}

	// wait until the interface thread finishes
	err = pthread_join(thread_, NULL);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_join\e[0;39m(): ") << strerror(err) << std::endl;
	}

	// destroy mutex
	err = pthread_mutex_destroy(&mutex_);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_mutex_destroy\e[0;39m(): ") << strerror(err) << std::endl;
	}

	// stop the UDP communication
	this->stop();
}
