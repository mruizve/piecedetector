/*
 * references:
 * https://www.binarytides.com/code-a-simple-socket-client-class-in-c/
 * http://cs.baylor.edu/~donahoo/practical/CSockets/code/BroadcastSender.c
 * https://gist.github.com/twslankard/1001201
 * https://linux.die.net/man/3/getaddrinfo
 */
#ifndef __SMF_NETWORK__
#define __SMF_NETWORK__

#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string>

#define MAGIC_MAX_BUFFER_LENGTH 256

namespace smf
{
	namespace net
	{
		// this class cannot be instantiated
		class Magic
		{
			protected:
				Magic(void);
				~Magic(void);

				// initialize request and response data
				void initialize(const unsigned char* _req, const int _reqlen, const int _reslen);

				// dump a data buffer
				void bufferdump(const char* _prefix, const unsigned char* _buffer, const size_t _length) const;

			public:
				// constant pointer to the request buffer
				const unsigned char* req(void) const;

				// pointer to the response buffer
				unsigned char* res(void);

				// request data length
				size_t reqlen(void) const;

				// request data length
				size_t reslen(void) const;

				// dump magic buffers
				void dump(void) const;

				// response analysis
				virtual std::string analyze(ssize_t bytes) const;

			protected:
				size_t reqlen_;
				size_t reslen_;
				unsigned char req_[MAGIC_MAX_BUFFER_LENGTH];
				unsigned char res_[MAGIC_MAX_BUFFER_LENGTH];
		};

		class UDP
		{
			protected:
				UDP(const std::string _ip, const std::string _port);
				~UDP(void);

			public:
				// start the UDP communication
				void start(void);

				// stop the UDP communication
				void stop(void);

				// send a string message
				void send(const std::string _msg);

				// receive a string message
				void recv(std::string& _msg);

			protected:
				// IP address
				std::string ip_;

				// port number
				unsigned short port_;

				// socket descriptor
				int sd_;

				// target address structure
				socklen_t targetlen_;
				struct sockaddr_in target_;
		};

		// keyword to trigger IP discovery by broadcast
		inline const std::string BROADCAST_DISCOVERY(void)
		{
			return "BROADCAST";
		}

		// validate the address and obtain the corresponding IP (if not already an IP)
		std::string validateAddress(const std::string _input);

		// validate and retrieve the port number
		unsigned short validatePort(const std::string _input);

		// cooperative discovery of UDP devices
		std::string discoverIP(const std::string _address, const std::string _port, Magic* _magic, const int _attempts);
	}
}

#endif
