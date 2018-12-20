#include<string.h>
#include<sstream>
#include "flexibowl.h"
#include "utils.h"

// definition of the request data and expected response size
smf::robots::Flexibowl::FlexibowlMagic::FlexibowlMagic(void)
{
	// see header file for the request and response description
	unsigned char req[] = 
	{
		0x80, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00,	0x47, 0x4f, 0x4c, 0x44
	};

	try
	{
		this->initialize(req, sizeof(req), 41);
	}
	catch( std::exception &e )
	{
		throw SMF_ERROR("caught exception during FlexibowlMagic instantiation\n>> " + std::string(e.what()));
	}
}

// dummy destructor
smf::robots::Flexibowl::FlexibowlMagic::~FlexibowlMagic(void)
{
}

// flexibowl broadcast response analysis
std::string smf::robots::Flexibowl::FlexibowlMagic::analyze(ssize_t _length) const
{
	// validate number of received bytes
	if( reslen_ != _length )
	{
		return std::string();
	}

	// validate header (?)
	if( memcmp(req_, res_, 5) )
	{
		return std::string();
	}

	// validate "GOLD" and "GOLD1"
	if( memcmp(req_+8, res_+10, 4) || memcmp(req_+8, res_+24, 4) || 0x31 != res_[28] )
	{
		return std::string();
	}

	// validate netmask (?) and standard LAN IP (?)
	unsigned char nm[] = {0xff, 0xff, 0x00, 0x00};
	unsigned char ip[] = {0xc0, 0xa8, 0x01, 0x01};
	if( memcmp(nm, res_+33, 4) || memcmp(res_+37, ip, 4) )
	{
		return std::string();
	}

	//this->dump();

	// extract and return the (guessed) flexibowl IP
	std::stringstream ss;
	ss << (int)res_[20] << "." << (int)res_[21] << "." << (int)res_[22] << "." << (int)res_[23];
	return ss.str();
}
