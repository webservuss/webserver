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
		parse_conf ex(file);


		for (int i = 0; i < ex.get_server().size(); ++i) {
			vector<int> p = ex.get_server_port(ex.get_server()[i]);
			std::cout << "Server: " << i << std::endl;
			for (vector<int>::iterator it = begin(p); it != end(p); ++it)
				std::cout << "port: " << *it << std::endl;
		}


	//	std::cout << "..." << ex.get_server_port(ex.get_server()[0])[0] << std::endl;
	//	std::cout << "..." << ex.get_server_port(ex.get_server()[0])[1] << std::endl;

		/*
		std::cout << "EX.GETservname(): |" << ex.get_server_name() << std::endl;
		std::cout << BLUE << "----------" << RESET << std::endl;
		std::cout << "sn  " << ex.get_server_name() <<std::endl;
		std::cout << "p  " << ex.get_port() <<std::endl;
		std::cout << "h  " << ex.get_host() <<std::endl;
		std::cout << "ep.front " << ex.get_error_page().front() <<std::endl;
		std::cout << "ep.back " << ex.get_error_page().back() <<std::endl;
		std::cout << "ai  " << ex.get_auto_index() <<std::endl;
		std::cout << "r  " << ex.get_root() <<std::endl;
		std::cout << "i  " << ex.get_index() <<std::endl;
		std::cout << "k  " << ex.get_key() <<std::endl;
		std::cout << "v  " << ex.get_value() <<std::endl;
		std::cout << BLUE << "----------" << RESET << std::endl;
		std::map<std::string, t_location> map_locations = ex.get_location();

		std::cout << "loc_map[\"/\"]._method: " << map_locations["/"]._method << std::endl;
		std::cout << "loc_map[*.error_image.png]._method: " << map_locations["*.error_image.png"]._method << std::endl;
		std::cout << "loc_map[*.ico]._method: " << map_locations["*.ico"]._method << std::endl;
		 */

	}
    else
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
    file.close();

    //HDE::parser_config_open r();
    HTTP::select_server t;
}
