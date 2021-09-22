#include "select_server.hpp"
#include "parser_conf.hpp"
#include "main.hpp"
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

static int check_argument_count(int argc)
{
    if(argc == 1)
	{
		std::cerr << "Choose a config file in the config file folder" << std::endl;
		return 1;
	}
    else if (argc > 2)
	{
		std::cerr << "Only one config file is allowed" << std::endl;
		return 1;
	}
    return 0;
}

static int check_input_argument(char **argv)
{
	struct stat s;
	if(stat(argv[1], &s) == 0)
	{
		if(s.st_mode & S_IFDIR)
		{
			std::cout << s.st_mode << std::endl;
			std::cerr << "Argument is not a file, but a directory" << std::endl;
			return 1;
		}
		else if(s.st_mode & S_IFREG)
		{
			std::string fn(argv[1]);
			if (!(fn.substr(fn.find_last_of(".") + 1) == "conf"))
			{
				std::cout << s.st_mode << std::endl;
				std::cerr << "Not a config file" << std::endl;
				return 1;
			}
		}
	}
	else
	{
		std::cout << "File doesn't exist" << std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (check_argument_count(argc) || check_input_argument(argv))
		return 1;

	HTTP::parse_conf ex(argv[1], argv);
	std::vector<HTTP::t_server> parser_servers = ex.get_server();
	HTTP::select_server t(ex.get_ports(), parser_servers);
}
