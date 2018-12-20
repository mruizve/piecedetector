#include<stdarg.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include "utils.h"

// executes a system command (using fork + execv) and retrieves the exit status
int smf::utils::exec(const std::string _cmd, ...)
{
	#define CMD_ARGS_MAX 256
	
	// validate input argument
	if( _cmd.empty() )
	{
		throw SMF_ERROR("command path cannot be \e[1;31mempty\e[0;39m");
	}
	
	// fork process
	pid_t pid = fork();
	if( 0 > pid )
	{
		throw SMF_ERROR("cannot create \e[1;31child process\e[0;39m (" + std::string(strerror(errno)) + ")");
	}

	// if we're on the child process, then
	if( 0 == pid )
	{
		// suppress the standard output and error files
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		// initialize list of variable arguments
		va_list ap;
		va_start(ap, _cmd);

		// generate input command arguments of the child
		char *argv[CMD_ARGS_MAX];
		argv[0] = (char*)_cmd.c_str();
		for( int i = 0; CMD_ARGS_MAX>i; i++)
		{
			argv[i+1] = va_arg(ap, char*);
			if( NULL == argv[i+1] )
			{
				break;
			}
		}

		// execute command
		(void)execv(argv[0], argv);

		// something went wrong
		throw SMF_ERROR("unexpected error from \e[1;31mexecl()\e[0;39m: " + std::string(strerror(errno)));
	}

	int child;
	if( 0 > waitpid(pid, &child, 0) )
	{
		throw SMF_ERROR("unexpected error from \e[1;31mwaitpid()\e[0;39m: " + std::string(strerror(errno)));
	}
	else
	{
		if ( WIFEXITED(child) )
		{
			return WEXITSTATUS(child);
		}
		else
		{
			throw SMF_ERROR("\e[1;31mchild\e[0;39m process died without exit status");
		}
	}

	#undef CMD_ARGS_MAX
}
