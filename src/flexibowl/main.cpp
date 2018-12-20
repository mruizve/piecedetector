#include<string.h>
#include<bitset>
#include<iostream>
#include<sstream>
#include "flexibowl.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	// validate input command line arguments
	if( 4 != argc )
	{
		std::cout
			<< "\nusage: " << argv[0] << " ip port time\n\n"
			<< "    ip: IP of the robot or 'BROADCAST' for cooperative discovery of it.\n"
			<< "  port: UDP port number.\n"
			<< "  time: control loop interval (in milliseconds).\n\n";
		return 0;
	}

	smf::robots::Flexibowl *flexibowl;
	try
	{
		int value = strtol(argv[3], NULL, 10);
		flexibowl = new smf::robots::Flexibowl(argv[1], argv[2], value);
	}
	catch(std::exception &e)
	{
		std::cerr << SMF_WHAT("cannot create the Flexibowl instance\n>> ") << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// commands loop
	int status;
	std::string cmd, echo;

	while( std::getline(std::cin, cmd) )
	{
		try
		{
			status = flexibowl->command(cmd);
		}
		catch( std::exception &e )
		{
			std::cerr << SMF_WHAT("cannot send " + cmd + " command to the flexibowl\n>> ") + e.what() << std::endl;
			continue;
		}
		
		if( status )
		{
			std::cout
				<< " --- " << std::bitset<32>(status) << std::endl
				<< " --- booting: " << (bool)(status & FLEXIBOWL_STATUS_BOOT) << ", "
				<< "error: " << (bool)(status & FLEXIBOWL_STATUS_ERROR) << ", "
				<< "init: " << (bool)(status & FLEXIBOWL_STATUS_INIT) << ", "
				<< "power: " << (bool)(status & FLEXIBOWL_STATUS_POWER) << ", "
				<< "ready: " << (bool)(status & FLEXIBOWL_STATUS_READY) << ", "
				<< "servo: " << (bool)(status & FLEXIBOWL_STATUS_SERVO) << ", " << std::endl;
		}
	}
	delete flexibowl;

	return EXIT_SUCCESS;
}
