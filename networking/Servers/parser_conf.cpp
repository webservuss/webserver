//
// Created by ruben on 13-07-21.
//

#include "parser_conf.hpp"

void parse_conf::set_values_server(std::string s)
{
	// this probably needs a better name than 'key'
	std::string key = s.substr(0, s.find(' '));
	if (key == "server_name")
		_server_name = s.substr(s.find(' ') + 1, s.size());
	if (key == "listen")
		_port = ft_stoi((s.substr(s.find(' ') + 1, s.size())));
	if (key == "host")
		_host = s.substr(s.find(' ') + 1, s.size());
	if (key == "error_page")
		_error_page = split(s.substr(s.find(' ') + 1, s.size()), ' ');
	if (key == "auto_index")
		_auto_index = ft_stoi(s.substr(s.find(' ') + 1, s.size()));
	if (key == "root")
		_root = s.substr(s.find(' ') + 1, s.size());
	if (key == "index")
		_index = s.substr(s.find(' ') + 1, s.size());
	if (key == "key")
		_key = s.substr(s.find(' ') + 1, s.size());
	if (key == "value")
		_value = s.substr(s.find(' ') + 1, s.size());
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
		location._autoindex= value;
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
	std::string map_key;
	while(std::getline(file, line, '\t'))
	{
		std::string key = line.substr(0, line.find(" "));
		line = line.substr(0, line.find('\n'));
		if (line.empty())
			continue;
		if (key == "location" && line[line.length() - 1] == '{') {
			is_acc = true;
			map_key = line.substr(line.find(' ') + 1, line.find('{') - line.find(' ') - 2); // set map_key for the new way
			_location_map[map_key];
		}
		else if (line[line.length() - 1] == '}') {
			is_acc = false;
		}

		std::cout << line << std::endl;
		if (!is_acc)
			set_values_server(line);
		if (is_acc) {
			// send the map with the appropriate key
			set_values_location(line, _location_map[map_key]); // for some reason it is not working yet
		}
	}
}

/* GETTERS */

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
