#include "../../old/parser_config.hpp"
#include "select_server.hpp"
#include "parser_conf.hpp"
#include <vector>

int main()
{
    std::cout<< RED<< "MAIN" << RESET << std::endl;
    std::ifstream file;
	const char *path = "configs/server.conf";
    file.open(path);
    if(!file.is_open()){
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
		return 0;
	}
    std::cout<< RED<< "###########CONFIG OPEN##########" << RESET << std::endl;
	HTTP::parse_conf ex(file);
	std::cout << "..." << ex.get_server_port(ex.get_server()[0])<< std::endl;
	std::cout << YELLOW << "..." << ex.get_server().size() << std::endl;
    file.close();

    // std::vector<int> ports;
    std::vector<HTTP::t_server> parser_servers = ex.get_server();

	std::vector<int> ports = ex.get_ports();
	for (unsigned long i = 0; i < ports.size(); i ++)
		std::cout << "PORT IS " << ports[i] << std::endl;
    std::cout << "about to call ma stuff" << std::endl;
    HTTP::select_server t(ports, parser_servers) ;
}


    //HDE::parser_config_open r();
    // int ports[3] = {80, 50, 70};
    // HTTP::select_server t1(ports, 3);
    // HTTP::select_server t2;

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