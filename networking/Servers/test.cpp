#include "select_server.hpp"
#include "parser_conf.hpp"
#include "test.hpp"
#include "test.hpp"
#include <dirent.h>
#include <stdlib.h>


int main(int argc, char **argv)
{












    // implement so it can handle different configs.
   
    std::string  pathname;
    for(int i= 1; i<argc; i++) 
        pathname = (argv[i]);
    if( argc == 1)
    {
        std::cout << "choos a config in configs"<<std::endl;
        exit(10);
    }
	HTTP::parse_conf ex(pathname.c_str());
    std::vector<HTTP::t_server> parser_servers = ex.get_server();
    HTTP::select_server t(ex.get_ports(), parser_servers) ;
}

std::vector<std::string>config()
{
    const char * record_dir_path = "configs";
    std::vector<std::string> files;
    struct dirent *entry;
	DIR *dir = opendir(record_dir_path);

	if (dir == NULL) 
	{
	  return files;
	}
	while ((entry = readdir(dir)) != NULL) 
	{
        std::cout << "open>?"<< std::endl;
		files.push_back(entry->d_name);
	}
	closedir(dir);
	
	return(files);
}