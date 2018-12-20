#include<sstream>
#include "utils.h"

// generates a nice error string, specially for debugging
const std::string  smf::utils::error(const std::string _file, const std::string _pretty, const std::string _fn, const int _line, const std::string _what)
{
	// extract method name (can be either a class constructor /
	// destructor, a class member or a standalone function)
	std::string fname;

	// remove arguments and qualifiers
	// case 1: class member
	std::size_t pname=_pretty.find("::"+_fn+"(");
	if( std::string::npos!=pname )
	{
		fname=_pretty.substr(0,pname)+"::"+_fn+"()";
	}
	else
	{
		// case 2: a function
		pname=_pretty.find(" "+_fn+"(");
		if( std::string::npos!=pname )
		{
			fname=_pretty.substr(0,pname)+" "+_fn+"()";
		}
		else
		{
			// case 3: constructor/destructor
			fname=_pretty.substr(0,pname)+_fn+"()";
			fname=_pretty.substr(0,pname)+_fn+"()";
		}
	}

	// remove return type, if any
	std::size_t space=fname.rfind(" ");
	if( std::string::npos!=space )
	{
		fname=fname.substr(space+1,fname.length()-space);
	}
	
	// generate error message
	std::stringstream ss;
	ss << "[\e[1;90m" + fname + " @ " + _file + ":" << _line << "\e[0;39m] " + _what;
	return ss.str();
}
