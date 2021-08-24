#include "select_server.hpp"
#include "parser_conf.hpp"

int main()
{
    // implement so it can handle different configs.
	const char *path = "configs/server.conf";
	HTTP::parse_conf ex(path);
    std::vector<HTTP::t_server> parser_servers = ex.get_server();
    HTTP::select_server t(ex.get_ports(), parser_servers) ;
}
