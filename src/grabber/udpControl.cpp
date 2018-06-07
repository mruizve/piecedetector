// reference:
// https://www.binarytides.com/code-a-simple-socket-client-class-in-c/

#include<iostream>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main(int argc, char *argv[])
{
	if( 3 != argc )
	{
		std::cout << "usage: " << argv[0] << " ip port" << std::endl;
		return 0;
	}

    //create socket
    int sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( 0>sd )
    {
		std::cerr << "[" << argv[0] << ":EE] " << "could not create socket" << std::endl;
		return -1;
    }

    // setup address structure
	struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    if( -1 == inet_addr(argv[1]))
    {
        //resolve the hostname, its not an ip address
        struct hostent *he = gethostbyname(argv[1]);
        if( NULL == he )
        {
			close(sd);
            std::cerr << "[" << argv[0] << ":EE] " << "failed to resolve hostname" << std::endl;
            return -1;
        }
         
        // cast the h_addr_list to in_addr (h_addr_list has the ip address in long format only)
        struct in_addr **addr_list = (struct in_addr**) he->h_addr_list;
 
        for( int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];
            std::cout << "[" << argv[0] << ":II] " << argv[1] << " resolved to " << inet_ntoa(server.sin_addr) << std::endl;             
            break;
        }
    }
	else
    {
		server.sin_addr.s_addr = inet_addr(argv[1]);
    }
     
    // connect to the server
    if( 0>connect(sd, (struct sockaddr*)&server, sizeof(server)) )
    {
		close(sd);
		std::cerr << "[" << argv[0] << ":EE] " << "connection error" << std::endl;
		return -1;
    }

	// commands loop
	char echo[4096];
	std::string cmd;
	socklen_t serverlen = sizeof(server);
	while( std::getline(std::cin, cmd) )
	{
		cmd = cmd + '\r';

		if( 0>sendto(sd, cmd.c_str(), cmd.length(), 0, (struct sockaddr*) &server, serverlen) )
		{
			close(sd);
			std::cerr << "[" << argv[0] << ":EE] " << "cannot send command \"" << cmd.c_str() << "\"" << std::endl;
			return -1;
		}

		memset(echo,0,sizeof(echo));
        if( 0>recvfrom(sd, echo, sizeof(echo), 0, NULL, NULL) )
        {
			close(sd);
			std::cerr << "[" << argv[0] << ":EE] " << "cannot receive echo of command \"" << cmd.c_str() << "\"" << std::endl;
			return -1;
        }
        char *cr=strrchr(echo,0x0d);
        cr[0] = ';';
        cr[1] = ' ';

        std::cout << echo << std::endl;
	}

	// close socket
	close(sd);

	return 0;
}
