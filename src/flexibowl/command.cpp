#include<string.h>
#include<iostream>
#include<sstream>
#include "flexibowl.h"
#include "utils.h"

// send a command to the flexibowl and provide the answer, if any
int smf::robots::Flexibowl::command(const std::string _cmd)
{
	int err;
	std::string echo;
	std::string answer;
	std::string response;
	std::stringstream ss;

	// disable cancellation before mutex lock
	err = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_setcancelstate\e[0;39m(): ") << strerror(err) << std::endl;
	}

	// lock mutex
	err = pthread_mutex_lock(&mutex_);
	if( 0 != err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mpthread_mutex_lock\e[0;39m(): " + std::string(strerror(err)));
	}

	// send command to the flexibowl
	try
	{
		this->UDP::send(_cmd+'\r');
	}
	catch( std::exception &e )
	{
		(void)pthread_mutex_unlock(&mutex_);
		throw SMF_ERROR("cannot send command to the Flexibowl\n>> " + std::string(e.what()));
	}

	// retrieve flexibowl response
	try
	{
		this->UDP::recv(response);
	}
	catch( std::exception &e )
	{
		(void)pthread_mutex_unlock(&mutex_);
		throw SMF_ERROR("cannot receive response from the Flexibowl\n>> " + std::string(e.what()));
	}

	// unlock mutex
	err = pthread_mutex_unlock(&mutex_);
	if( 0 != err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mpthread_mutex_unlock\e[0;39m(): " + std::string(strerror(err)));
	}

	// enable cancellation after mutex unlock
	err = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if( 0 != err )
	{
		std::cerr << SMF_WHAT("unexpected error from \e[1;31mpthread_setcancelstate\e[0;39m(): ") << strerror(err) << std::endl;
	}

	// extract response tokens
	ss << response;
	std::getline(ss, echo, '\r');
	std::getline(ss, answer, '\r');

	// validate echo token
	if( _cmd.compare(echo) )
	{
		throw SMF_ERROR("\e[1;31munexpected\e[0;39m echo in response (echo: " + echo + ")");
	}

	// validate answer
	if( 0 == answer.size() )
	{
		throw SMF_ERROR("\e[1;31munexpected\e[0;39m empty answer in response");
	}

	// if there's no answer, then
	if( 1 == answer.size() )
	{
		// we're done
		return 0;
	}

	// clear the string stream object
	ss.str("");
	ss.clear();
	
	// return the numerical value of the answer
	int value;
	ss << answer;
	ss >> value;
	return value;
}

