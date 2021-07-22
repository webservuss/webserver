#include "parser_config.hpp"
#include "select_server.hpp"
#include "parser_conf.hpp"

int main()
{
    std::cout<< RED<< "MAIN" << RESET << std::endl;
    std::ifstream file;
	const char *path = "configs/server2.conf";
    file.open(path);
    if(file.is_open()){
        std::cout<< RED<< "###########CONFIG OPEN##########" << RESET << std::endl;
		parse_conf conf(file);

		for (int i = 0; i < conf.get_server().size(); ++i) {
			vector<int> p = conf.get_server_port(conf.get_server()[i]);
			std::cout << "Server: " << i << std::endl;
			for (vector<int>::iterator it = p.begin(); it != p.end(); ++it)
				std::cout << "port: " << *it << std::endl;
		}
		vector<t_server> a = conf.get_server();
		std::cout<< a[0]._server_name[0] << std::endl;

	}
    else
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
    file.close();



    //HDE::parser_config_open r();
   HTTP::select_server t;
}
