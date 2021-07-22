//
// Created by ruben on 13-07-21.
//

#ifndef __PARSER_CONF_HPP__
#define __PARSER_CONF_HPP__

#include "../../all_libs.hpp"
#include "../../http.hpp"

using namespace std;
typedef struct 				s_location {
	string 					_address; //this one is replaced by the map-key
	string 					_method;
	string 					_root;
	string 					_cgi;
	bool 					_autoindex;
	int						_client_body_size;
}							t_location;

typedef struct				s_server {
	vector<string>			_server_name;
	vector<int>				_port;
	string					_host;
	vector<string>			_error_page;
	bool       				_autoindex;
	string					_root;
	string					_index;
	string					_key;
	string					_value;
	map<string, t_location> _location_map;
}							t_server;



class parse_conf
{
private:
<<<<<<< HEAD
	vector<t_server> _server;
=======
	/*
	std::string _server_name;
	int         _port;
	std::string _host;
	std::vector<std::string> _error_page;
	int         _auto_index;
	std::string _root;
	std::string _index;
	std::string _key;
	std::string _value;

	std::map<std::string, t_location> _location_map;
	 */

	//private
	std::vector<t_server> _server;
>>>>>>> sophie_request

	void set_values_server(string s, t_server &server);
	void set_values_location(string s, t_location &location);
public:
	parse_conf(ifstream &file);

	/* GETTERS */
	const vector<int>		&get_server_port(const t_server &server);
	const vector<t_server>	&get_server() const;
	/*
	const std::string &get_server_name() const;
	int get_port() const;
	const std::string &get_host() const;
	const std::vector<std::string> &get_error_page() const;
	int get_auto_index() const;
	const std::string &get_root() const;
	const std::string &get_index() const;
	const std::string &get_key() const;
	const std::string &get_value() const;


	const std::map<std::string, t_location> &get_location() const;
	*/
};

#endif //__PARSER_CONF_HPP__
