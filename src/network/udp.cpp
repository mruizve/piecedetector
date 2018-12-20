#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include "network.h"
#include "utils.h"

smf::net::UDP::UDP(const std::string _ip, const std::string _port)
{
	try
	{
		// validate address and retrieve IP
		ip_ = smf::net::validateAddress(_ip);
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("\e[1;31m" + _ip + "\e[0;39m invalid host name or IPv4 address\n>> " + std::string(e.what()));
	}

	try
	{
		// validate and store port number
		port_ = smf::net::validatePort(_port);
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("\e[1;31m" + _port + "\e[0;39m invalid destination port number\n>> " + std::string(e.what()));
	}
}

smf::net::UDP::~UDP(void)
{
}

void smf::net::UDP::start(void)
{
	// create the UDP socket
    sd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( 0>sd_ )
    {
		throw SMF_ERROR("unexpected error from \e[1;31msocket()\e[0;39m: " + std::string(strerror(errno)));
    }

	// construct address structure of the target
    target_.sin_family = AF_INET;
    target_.sin_port = htons(port_);
	int err = inet_pton(AF_INET, ip_.c_str(), &(target_.sin_addr));
	if( 0 > err  )
	{
		(void)close(sd_);
		throw SMF_ERROR("unexpected error from \e[1;31minet_pton()\e[0;39m: " + std::string(strerror(errno)));
	}
	if( 0 == err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31minet_pton()\e[0;39m: \e[1;31m" + ip_ + "\e[0;39m is not a valid network address");
	}
}

void smf::net::UDP::stop(void)
{
	// close the UDP socket
	(void)close(sd_);
}

void smf::net::UDP::send(const std::string _msg)
{
	// initialize target structure size
	targetlen_ = sizeof(target_);

	// send string message to the target
	if( 0 > sendto(sd_, _msg.c_str(), _msg.length(), 0, (struct sockaddr*) &target_, targetlen_) )
	{
		throw SMF_ERROR("unexpected error from \e[1;31msendto()\e[0;39m: " + std::string(strerror(errno)));
	}
}

void smf::net::UDP::recv(std::string& _msg)
{
	#define MSG_MAX_LENGTH 4096

	// initialize target structure size
	targetlen_ = sizeof(target_);

	// data buffer initialization
	char buffer[MSG_MAX_LENGTH];

	// receive message from the target
	ssize_t bytes = recvfrom(sd_, buffer, MSG_MAX_LENGTH, 0, (struct sockaddr*) &target_, &targetlen_);
	if( 0 >= bytes )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mrecvfrom()\e[0;39m: " + std::string(strerror(errno)));
	}

	// copy buffer to the output string
	_msg.assign(buffer, bytes);

	#undef MSG_MAX_LENGTH
}
