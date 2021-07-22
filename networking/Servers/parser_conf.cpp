//
// Created by ruben on 13-07-21.
//

#include "parser_conf.hpp"

using namespace std;

void set_port(string &value, vector<int> &port)
{
	vector<int> elem = split_stoi(value, ' ');
	for (vector<int>::iterator it = elem.begin(); it != elem.end(); ++it) {
		port.push_back(*it);
	}
}

void parse_conf::set_values_server(std::string s, t_server &server)
{
	// this probably needs a better name than 'key'
	//server._port = split_stoi(value, ' ');
	try
	{
		std::string key = s.substr(0, s.find(' '));
		std::string value = s.substr(s.find(' ') + 1, s.find(';') - s.find(' ') - 1);
		if (key == "server_name")
			server._server_name = split(value, ' ');
		else if (key == "port")
			set_port(value, server._port);
		else if (key == "host")
			server._host = value;
		else if (key == "error_page")
			server._error_page = split(value, ' ');
		else if (key == "autoindex")
			server._autoindex = (value == "on") ? true : false;
		else if (key == "root")
			server._root = value;
		else if (key == "index")
			server._index = value;
		else if (key == "key")
			server._key = value;
		else if (key == "value")
			server._value = s.substr(s.find(' ') + 1, s.size());
		else
			throw (s);
	}
	catch (string &n)
	{
		std::cout << "Unknown setting: " << n << std::endl;
	}
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
		location._autoindex= (value == "on");
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
	while(std::getline(file, line, '\n'))
	{
		line = trim_whitespace_front(line);
		if (line.empty()) {
			std::cout << "empty line" << std::endl;
			continue;
		}
		std::string key = line.substr(0, line.find(' '));
		//line = line.substr(0, line.find('\n'));
		//std::cout << "l|" << line << "|"<< std::endl;
		//std::cout << "k|" << key << "|"<< std::endl;
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
		if (!is_acc) {
			cout << "setting server, sc: " << server_count << endl;
			set_values_server(line, _server[server_count - 1]);
		}
		if (is_acc) {
			// send the map with the appropriate key
			set_values_location(line, _server[server_count - 1]._location_map[map_key]); // for some reason it is not working yet
		}
	}
}

const std::vector<t_server> &parse_conf::get_server() const
{
    //the total config file
	return _server;
}

// gaat dit werken?
const std::vector<int>	&parse_conf::get_server_port(const t_server &server) {
	return server._port;
}

in

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


