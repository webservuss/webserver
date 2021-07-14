#include "parser_config.hpp"
#include "select_server.hpp"
#include "parser_conf.hpp"

int main()
{
    std::cout<< RED<< "MAIN" << RESET << std::endl;
    std::ifstream file;
    // std::ifstream file("server.conf");
    //std::string path;
    //path = "configs/server.conf";
	const char *path = "configs/server.conf";
	// file.open(path , std::ifstream::in);
    file.open(path);
    if(file.is_open()){
        std::cout<< RED<< "###########CONFIG OPEN##########" << RESET << std::endl;
        //HDE::parser_config parser (file);
		//std::cout<< BLUE << "getHost " << parser.getHost() << RESET  << std::endl;
		parse_conf ex(file);
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
//		std::cout << "l[0]: " << ex._location[0]._method << std::endl;
//		std::cout << "l[1]: " << ex._location[1]._method << std::endl;
//		std::cout << "l[2]: " << ex._location[2]._method << std::endl;
//		std::cout << "l[0]: " << ex._location[0]._root << std::endl;
//		std::cout << "l[1]: " << ex._location[1]._client_body_size << std::endl;
		std::vector<t_location> example = ex.get_location();
		std::map<std::string, t_location> map_locations = ex.get_location_map();

		std::cout << "l[0]: |" << example[0]._address << "|" << std::endl;
		std::cout << "l[1]: |" << example[1]._address << "|" << std::endl;
		std::cout << "l[2]: |" << example[2]._address << "|" << std::endl;

		std::cout << "loc_map[\"/\"]._method: " << map_locations["/ {"]._method << std::endl;
		std::cout << "loc_map[*.error_image.png {]._method: " << map_locations["*.error_image.png {"]._method << std::endl;
		std::cout << "loc_map[*.ico {]._method: " << map_locations["*.ico {"]._method << std::endl;

	}
    else
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
    file.close();

    //HDE::parser_config_open r();
    HTTP::select_server t;
}
