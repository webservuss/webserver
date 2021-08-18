#ifndef __PARSER_CONF_HPP__
#define __PARSER_CONF_HPP__

#include "../../all_libs.hpp"
#include "../../http.hpp"
#include "../Colors.hpp"
namespace HTTP {

typedef struct s_location {
	std::string _address; //this one is replaced by the map-key
	std::string _method;
	std::string _root;
	std::string _cgi;
	std::string _autoindex;
	int 		_client_body_size;
}				t_location;

typedef struct							s_server {
	std::string							_server_name;
	int        							_port;
	std::string							_host;
	std::vector<std::string>			_error_page;
	int        							_auto_index;
	std::string							_root;
	std::string							_index;
	std::string							_key;
	std::string							_value;
	std::map<std::string, t_location>	_location_map;
}										t_server;



class parse_conf
{
private:
	std::vector<t_server> _server;
	void set_values_server(std::string s, t_server &server);
	void set_values_location(std::string s, t_location &location);

public:
	parse_conf(std::ifstream &file);

	/* GETTERS */
	int							get_server_port(const t_server &server);
	const std::vector<t_server>	&get_server() const;
	std::vector<int>			get_ports();

};
}
#endif //__PARSER_CONF_HPP__
