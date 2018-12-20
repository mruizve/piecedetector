#ifndef __SMF_UTILS__
#define __SMF_UTILS__

#include<stdexcept>

#define SMF_WHAT(what) \
	smf::utils::error(__FILE__+__FILE_OFFSET__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, what)

#define SMF_ERROR(what) \
	std::runtime_error(SMF_WHAT(what))

namespace smf
{
	namespace utils
	{
		// generates a nice error string, specially for debugging
		const std::string error(const std::string _file, const std::string _pretty, const std::string _fn, const int _line, const std::string _what);

		// executes a system command (using fork + execv) and retrieves the exit status
		int exec(const std::string _cmd, ...);
	}
}

#endif 
