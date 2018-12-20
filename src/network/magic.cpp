#include<string.h>
#include<sstream>
#include "network.h"
#include "utils.h"


smf::net::Magic::Magic(void)
{
}

smf::net::Magic::~Magic(void)
{
}

// initialize request and response data
void smf::net::Magic::initialize(const unsigned char* _req, const int _reqlen, const int _reslen)
{
	// validate input arguments
	if( NULL == _req )
	{
		throw SMF_ERROR("invalid request data buffer: cannot be \e[1;31mNULL\e[0;39m");
	}
	if( 0 >= _reqlen )
	{
		throw SMF_ERROR("invalid request data length: cannot be \e[1;31mless than\e[0;39m zero");
	}
	if( MAGIC_MAX_BUFFER_LENGTH < _reqlen )
	{
		std::stringstream ss;
		ss << MAGIC_MAX_BUFFER_LENGTH;
		throw SMF_ERROR("invalid request data length: cannot be \e[1;31mgreater than\e[0;39m " + ss.str());
	}
	if( 0 >= _reslen )
	{
		throw SMF_ERROR("invalid response data length: cannot be \e[1;31mless than\e[0;39m zero");
	}
	if( MAGIC_MAX_BUFFER_LENGTH < _reslen )
	{
		std::stringstream ss;
		ss << MAGIC_MAX_BUFFER_LENGTH;
		throw SMF_ERROR("invalid response data length: cannot be \e[1;31mgreater than\e[0;39m" + ss.str());
	}

	// set internal data
	reqlen_ = _reqlen;
	reslen_ = _reslen;
	memcpy(req_, _req, _reqlen);
}

// dump a data buffer
void smf::net::Magic::bufferdump(const char* _prefix, const unsigned char* _buffer, const size_t _length) const
{
	printf("%s [%04dB]: ", _prefix);
	for(int i=0; _length>i; i++)
	{
		printf(" %02x ", _buffer[i]);
	}
	printf("\n");

	printf("%s [%04dB]: ", _prefix);
	for(int i=0; _length>i; i++)
	{
		printf("%3d ", _buffer[i]);
	}
	printf("\n");

	printf("%s [%04dB]: ", _prefix);
	for(int i=0; _length>i; i++)
	{
		if( isprint(_buffer[i]) )
		{
			printf("%3c ", _buffer[i]);
		}
		else
		{
			printf("--- ");
		}
		
	}
	printf("\n");
}

// constant pointer to the request buffer
const unsigned char* smf::net::Magic::req(void) const
{
	return req_;
}

// pointer to the response buffer
unsigned char* smf::net::Magic::res(void)
{
	return res_;
}

// request data length
size_t smf::net::Magic::reqlen(void) const
{
	return reqlen_;
}

// request data length
size_t smf::net::Magic::reslen(void) const
{
	return reslen_;
}

// dump magic buffers
void smf::net::Magic::dump(void) const
{
	// dump request
	this->bufferdump(" request", req_, reqlen_);

	// dump response
	this->bufferdump("response", res_, reslen_);
}

// response analysis
std::string smf::net::Magic::analyze(ssize_t bytes) const
{
	throw SMF_ERROR("virtual method \e[1;31manalyze()\e[0;39m must be defined on derived classes");
}
