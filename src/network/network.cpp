#include<limits.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
#include<sstream>
#include "network.h"
#include "utils.h"

// validate the address and obtain the corresponding IP (if not already an IP)
std::string smf::net::validateAddress(const std::string _input)
{
	if( _input.empty() )
	{
		throw SMF_ERROR("string cannot be \e[1;31mempty\e[0;39m");
	}

	// if the IP will not be determined by broadcast, then
	if( _input.compare(smf::net::BROADCAST_DISCOVERY()) )
	{
		int err;

		// retrieve the corresponding IP address
		struct addrinfo *results;
		err = getaddrinfo(_input.c_str(), NULL, NULL, &results);
		if( 0 != err )
		{
			throw SMF_ERROR("unexpected error from \e[1;31mgetaddrinfo()\e[0;39m: " + std::string(gai_strerror(err)));
		}

		// initialize output
		std::string output(INET6_ADDRSTRLEN, 0);

		// for each address structure,
		for( struct addrinfo *res = results; NULL != res; res = res->ai_next)
		{
			// if the address family is not IPv6, then
			if( AF_INET == res->ai_family )
			{
				// retrieve the first valid IP address and store it on the output string
				struct in_addr *address = &((struct sockaddr_in*)res->ai_addr)->sin_addr;
				if( NULL != inet_ntop(AF_INET, address, (char*)output.data(), INET6_ADDRSTRLEN) )
				{
					break;
				}
			}
		}

		// release resources allocated by getaddrinfo()
		freeaddrinfo(results);

		return output;
	}
	else
	{
		// IP address need to be discovered by broadcast
		return _input; // = BROADCAST_DISCOVERY
	}
}

// validate and retrieve the port number
unsigned short smf::net::validatePort(const std::string _input)
{
	// validate port string
	if( _input.empty() )
	{
		throw SMF_ERROR("string cannot be \e[1;31mempty\e[0;39m");
	}

	// retrieve and validate port number
	char *port;
	long number = strtol(_input.c_str(), &port, 10);
	if( 0 != *port )
	{
		throw SMF_ERROR("\e[1;31mnot\e[0;39m a number");
	}
	if( 0 > number || USHRT_MAX < number)
	{
		throw SMF_ERROR("port number \e[1;31mout of range\e[0;39m");
	}

	// store port number
	return (unsigned short)number;
}

// cooperative discovery of UDP devices
std::string smf::net::discoverIP(const std::string _address, const std::string _port, smf::net::Magic* _magic, const int _attempts)
{
	std::string ip;
	try
	{
		// validate broadcast address and retrieve IP
		ip = smf::net::validateAddress(_address);
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("\e[1;31m" + _address + "\e[0;39m invalid broadcast address\n>> " + std::string(e.what()));
	}

	unsigned short port;
	try
	{
		// validate and store port number
		port = smf::net::validatePort(_port);
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("\e[1;31m" + _port + "\e[0;39m invalid broadcast port number\n>> " + std::string(e.what()));
	}

	// create the UDP socket
	int sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( 0 > sd )
	{
		throw SMF_ERROR("unexpected error from \e[1;31msocket()\e[0;39m: " + std::string(strerror(errno)));
	}

	// set broadcast option
	int err = 1;
	if( 0 > setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &err, sizeof(err)) )
	{
		throw SMF_ERROR("unexpected error from \e[1;31msetsockopt()\e[0;39m: " + std::string(strerror(errno)));
	}

	// set the receiving timeout option
	struct timeval tv = {.tv_sec = 0, .tv_usec = 500000};
	if( 0 > setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) )
	{
		throw SMF_ERROR("unexpected error from \e[1;31msetsockopt()\e[0;39m: " + std::string(strerror(errno)));
	}

	// construct source address structure
	struct sockaddr_in src;
	memset(&src, 0, sizeof(src));
	src.sin_family = AF_INET;
	src.sin_port = htons(port);

	// bind source port
	// (this tells the OS for which incoming port to actually accept
	// packets, all packets to other ports are dropped away)
	if( 0 > bind(sd, (struct sockaddr*)&src, sizeof(src)) )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mbind()\e[0;39m: " + std::string(strerror(errno)));
	}

	// construct destination address structure
	struct sockaddr_in dst;
	memset(&dst, 0, sizeof(dst));
	dst.sin_family = AF_INET;
	dst.sin_port = htons(port);
	err = inet_pton(AF_INET, ip.c_str(), &(dst.sin_addr));
	if( 0 > err  )
	{
		throw SMF_ERROR("unexpected error from \e[1;31minet_pton()\e[0;39m: " + std::string(strerror(errno)));
	}
	if( 0 == err )
	{
		throw SMF_ERROR("unexpected error from \e[1;31minet_pton()\e[0;39m: \e[1;31m" + ip + "\e[0;39m is not a valid network address");
	}

	// output IP initialization
	ip = std::string("\e[1;31mnot found\e[0;39m");

	for( int i=0, done = 0; !done && _attempts>i; i++ )
	{
		// broadcast data through the network
		if( _magic->reqlen() != sendto(sd, _magic->req(), _magic->reqlen(), 0, (struct sockaddr*)&dst, sizeof(dst)) )
		{
			throw SMF_ERROR("unexpected error from \e[1;31msendto()\e[0;39m: " + std::string(strerror(errno)));
		}

		std::stringstream ss;
		ss << "\e[1;90m(" << (i+1) << "/" << _attempts << ")\e[0;39m";
		std::cout << SMF_WHAT("performing \e[1;34mnetwork discovery\e[0;39m, waiting for a response " + ss.str() + "...") << std::endl;

		ssize_t bytes;
		for( int j=0; !done && _attempts>j; j++ )
		{
			// receive broadcast messages
			socklen_t dstlen = sizeof(dst);
			if( 0 > (bytes = recvfrom(sd, _magic->res(), _magic->reslen(), 0, (struct sockaddr*) &dst, &dstlen)) )
			{
				if( EAGAIN != errno && EWOULDBLOCK != errno )
				{
					throw SMF_ERROR("unexpected error from \e[1;31mrecvfrom()\e[0;39m: " + std::string(strerror(errno)));
				}
				else
				{
					std::cout << SMF_WHAT("discovery response timeout \e[1;34mexpired\e[0;39m, retrying...") << std::endl;
					continue;
				}
			}

			// analyze message
			std::string outcome = _magic->analyze(bytes);

			// if the outcome string is not empty, then
			if( !outcome.empty() )
			{
				// retrieve the desired IP
				ip = outcome;

				// stop all loops
				done = !done;

				// get away from here
				break;
			}
		}
	}

	// close socket
	(void)close(sd);

	return ip;
}
