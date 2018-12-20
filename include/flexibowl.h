/*
 * references:
 * https://gist.github.com/aallan/b4bb86db86079509e6159810ae9bd3e4
 * https://weseetips.wordpress.com/tag/exception-from-constructor-initializer-list/
 * 
 * 
 * UDP request for collaborative discovery of the flexibowl:
 *
 *      header (?)     |   unknown   |    magic word   
 * --------------------|-------------|-----------------
 *  80  00  00  01  00 |  04  00  00 |  47  4f  4c  44 
 * 128   0   0   1   0 |   4   0   0 |  71  79  76  68 
 * --- --- --- --- --- | --- --- --- |   G   O   L   D 
 *
 *
 * UDP response from the flexibowl:
 *      header (?)     |       unknown       |    magic word   |       MAC address       |   IP address    |     magic word      |     unknown     |   netmask (?)   |   LAN IP (?)
 * --------------------|---------------------|-----------------|-------------------------|-----------------|---------------------|-----------------|-----------------|----------------
 *  80  00  00  01  00 |  1f  00  00  00  00 |  47  4f  4c  44 |  10  18  9e  00  25  36 |  0a  0a  ee  26 |  47  4f  4c  44  31 |  00  bf  32  9a |  ff  ff  00  00 |  c0  a8  01  01
 * 128   0   0   1   0 |  31   0   0   0   0 |  71  79  76  68 |  16  24 158   0  37  54 |  10  10 238  38 |  71  79  76  68  49 |   0 191  50 154 | 255 255   0   0 | 192 168   1   1
 * --- --- --- --- --- | --- --- --- --- --- |   G   O   L   D | --- --- --- ---   %   6 | --- --- ---   & |   G   O   L   D   1 | --- ---   2 --- | --- --- --- --- | --- --- --- ---
 *
 *
 * status answer: integer (4 bytes)
 * 0000 0000 0SB0 0R00 AA0I 000X 0X0S EEEE
 *            ||   |   || |    |  | | '+++-> error word: the LSB is set on error. in the case of
 *            ||   |   || |    |  | |                    error, the code equals (EEE1 & 0x1110).
 *            ||   |   || |    |  | |                    the documentation of codes meaning seems
 *            ||   |   || |    |  | |                    wrong since code == 0x0110 implies that
 *            ||   |   || |    |  | |                    the actuator is switch off.
 *            ||   |   || |    |  | |
 *            ||   |   || |    |  | '-> servo bit 2: same as servo bit 1.
 *            ||   |   || |    |  |
 *            ||   |   || |    |  '-> arcane bit 2: set when the servo is enabled and the
 *            ||   |   || |    |                    actuator is switch off and turned on
 *            ||   |   || |    |                    again. it is cleared again when the
 *            ||   |   || |    |                    the servo motor is re-enabled.
 *            ||   |   || |    |
 *            ||   |   || |    '-> arcane bit 1: it is set to zero at boot, regardless the
 *            ||   |   || |                      power state of the actuator. it is set to
 *            ||   |   || |                      one when the servo motor is enabled for
 *            ||   |   || |                      the first time (remains set forever).
 *            ||   |   || |
 *            ||   |   || '-> initialized bit: set when the flexiblowl has been properly
 *            ||   |   ||                      initialized. cleared after a KL command
 *            ||   |   ||                      or (temporarily) during boot sequence.
 *            ||   |   ||
 *            ||   |   '+-> actuator bits: set/cleared when the
 *            ||   |                       actuator is powered/off.
 *            ||   |
 *            ||   '-> ready bit: cleared when the flexibowl executes
 *            ||                  a command. it is also cleared when
 *            ||                  the servo motor is disabled or the
 *            ||                  actuator is switch off.
 *            ||
 *            |'-> booting bit: set for a small transient time after
 *            |                 power on (it is cleared when the status
 *            |                 LED becomes green after power on).
 *            |
 *            '-> servo bit 1: set/cleared when the servo motor is
 *                             enabled/disabled. it is also cleared 
 *                             when the actuator is switch off.
 */
#ifndef __SMF_ROBOTS_FLEXIBOWL__
#define __SMF_ROBOTS_FLEXIBOWL__

#include<pthread.h>
#include<signal.h>
#include "network.h"

#define FLEXIBOWL_STATUS_BOOT  0x00200000
#define FLEXIBOWL_STATUS_ERROR 0x0000000f
#define FLEXIBOWL_STATUS_INIT  0x00001000
#define FLEXIBOWL_STATUS_POWER 0x0000c000
#define FLEXIBOWL_STATUS_READY 0x00040000
#define FLEXIBOWL_STATUS_SERVO 0x00400010

namespace smf
{
	namespace robots
	{
		// real-time signal used to trigger the controller thread
		inline const int SIGNAL(void)
		{
			return SIGRTMIN;
		}

		class Flexibowl: public smf::net::UDP
		{
			protected:
				class FlexibowlMagic: public smf::net::Magic
				{
					public:
						FlexibowlMagic(void);
						~FlexibowlMagic(void);

						// analyze broadcast responses for collaborative discovery
						std::string analyze(ssize_t _length) const;
				};

				// initialize and launch the interface thread
				void interface(void);
				static void* interface_helper(void* _robot);

			public:
				Flexibowl(const std::string _ip, const std::string _port, int _interval);
				~Flexibowl(void);

				// send command to the flexibowl
				int command(std::string _cmd);

				// restart flexibowl state and reset internal controller
				int initialize(void);

			protected:
				int interval_;
				sigset_t sigset_;
				pthread_t thread_;
				pthread_mutex_t mutex_;
		};
	}
}

#endif
