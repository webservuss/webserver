//
// Created by ruben on 13-07-21.
//

#ifndef __PARSER_CONF_HPP__
#define __PARSER_CONF_HPP__

#include "../../all_libs.hpp"

typedef struct s_location {
	std::string _address;
	std::string _method;
	std::string _root;
	std::string _cgi;
	std::string _autoindex;

	int _client_body_size;
}				t_location;
#include "../../http.hpp"


class parse_conf
{
private:
	std::vector<t_location> _location;
	std::string _server_name;
	int         _port;
	std::string _host;
	std::vector<std::string> _error_page;
	int         _auto_index;
	std::string _root;
	std::string _index;
	std::string _key;
	std::string _value;

	//TODO: better datastructure? Map? 2d Vector of strings?

	void set_values_server(std::string s);
	void set_values_location(std::string s, int i);
public:
	parse_conf(std::ifstream &file);
	std::vector<std::string> split(const std::string &s, char delim);

	/* GETTERS */
	const std::string &get_server_name() const;
	int get_port() const;
	const std::string &get_host() const;
	const std::vector<std::string> &get_error_page() const;
	int get_auto_index() const;
	const std::string &get_root() const;
	const std::string &get_index() const;
	const std::string &get_key() const;
	const std::string &get_value() const;

	// TODO get's total vector.
	const std::vector<t_location> &get_location() const;
};

#endif //__PARSER_CONF_HPP__