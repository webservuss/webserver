//
// Created by ruben on 13-07-21.
//

#include "parser_conf.hpp"

using namespace std;

void parse_conf::set_values_server(std::string s, t_server &server)
{
	// this probably needs a better name than 'key'
	std::string key = s.substr(0, s.find(' '));
	std::string value = s.substr(s.find(' ') + 1,s.find(';') - s.find(' ') - 1);
	if (key == "server_name")
		server._server_name = split(value, ' ');
	if (key == "port")
		server._port = split_stoi(value, ' ');
	if (key == "host")
		server._host = value;
	if (key == "error_page")
		server._error_page = split(value, ' ');
	if (key == "auto_index")
		server._auto_index = (value == "on") ? true : false;
	if (key == "root")
		server._root = value;
	if (key == "index")
		server._index = value;
	if (key == "key")
		server._key = value;
	if (key == "value")
		server._value = s.substr(s.find(' ') + 1, s.size());
}

// This function gets the whole line and a struct (t_location)
void	parse_conf::set_values_location(std::string s, t_location &location)
{
	// we should think of a better name then key
	std::string key = s.substr(0, s.find(' '));
	std::string value = s.substr(s.find(' ') + 1,s.find(';') - s.find(' ') - 1);
	if (key == "method")
		location._method= value;
	if (key == "root")
		location._root = value;
	if (key == "cgi")
		location._cgi = value;
	if (key == "autoindex")
		location._auto_index= (value == "on");
	if (key == "client_body_size")
		location._client_body_size = ft_stoi(value);
}

	parse_conf::parse_conf(std::ifstream &file)
{
	if (!file) {
		perror("No file");
		exit(1);
	}
	std::string line;
	// conf must have empty line at end?
	bool is_acc = false;
	int			server_count = 0;
	std::string map_key;
	while(std::getline(file, line, '\t'))
	{
		std::string key = line.substr(0, line.find(" "));
		line = line.substr(0, line.find('\n'));
		if (line.empty())
			continue;
		if (key == "server") {
			server_count++;
			_server.resize(server_count);
			continue;
		}
		if (key == "location" && line[line.length() - 1] == '{') {
			is_acc = true;
			map_key = line.substr(line.find(' ') + 1, line.find('{') - line.find(' ') - 2); // set map_key for the new way
			_server[server_count - 1]._location_map[map_key];
		}
		else if (line[line.length() - 1] == '}') {
			is_acc = false;
		}

		std::cout << line << std::endl;
		if (!is_acc)
			set_values_server(line, _server[server_count - 1]);
		if (is_acc) {
			// send the map with the appropriate key
			set_values_location(line, _server[server_count - 1]._location_map[map_key]); // for some reason it is not working yet
		}
	}
}

const std::vector<t_server> &parse_conf::get_server() const
{
	return _server;
}

// gaat dit werken?
const std::vector<int>	&parse_conf::get_server_port(const t_server &server) {
	return server._port;
}

/* GETTERS */

/*
const std::string &parse_conf::get_server_name() const
{
	return _server_name;
}

int parse_conf::get_port() const
{
	return _port;
}

const std::string &parse_conf::get_host() const
{
	return _host;
}

int parse_conf::get_auto_index() const
{
	return _auto_index;
}

const std::string &parse_conf::get_root() const
{
	return _root;
}

const std::string &parse_conf::get_index() const
{
	return _index;
}

const std::string &parse_conf::get_key() const
{
	return _key;
}

const std::string &parse_conf::get_value() const
{
	return _value;
}

const std::vector<std::string> &parse_conf::get_error_page() const
{
	return _error_page;
}

const std::map<std::string, t_location> &parse_conf::get_location() const
{
	return _location_map;
}
*/


