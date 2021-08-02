#include "parser_conf.hpp"

void HTTP::parse_conf::set_values_server(std::string s, t_server &server)
{
	std::string key = s.substr(0, s.find(' '));
	if (key == "server_name")
		server._server_name = s.substr(s.find(' ') + 1, s.size());
	if (key == "listen")
		server._port = ft_stoi((s.substr(s.find(' ') + 1, s.size())));
	if (key == "host")
		server._host = s.substr(s.find(' ') + 1, s.size());
	if (key == "error_page")
		server._error_page = split(s.substr(s.find(' ') + 1, s.size()), ' ');
	if (key == "auto_index")
		server._auto_index = ft_stoi(s.substr(s.find(' ') + 1, s.size()));
	if (key == "root")
		server._root = s.substr(s.find(' ') + 1, s.size() - s.find(' ') - 2);
	if (key == "index")
		server._index = s.substr(s.find(' ') + 1, s.size() - s.find(' ') - 2);
	if (key == "key")
		server._key = s.substr(s.find(' ') + 1, s.size());
	if (key == "value")
		server._value = s.substr(s.find(' ') + 1, s.size());
}

// This function gets the whole line and a struct (t_location)
void	HTTP::parse_conf::set_values_location(std::string s, t_location &location)
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

	HTTP::parse_conf::parse_conf(std::ifstream &file)
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
		std::string s1;
		int i = 0;
		while (line[i] == '\t' || line[i] == '\n')
			i++;
		s1 = line.substr(i, line.size() - i);
		std::string key = s1.substr(0, s1.find(" "));
		s1 = s1.substr(0, s1.find('\n'));
		if (s1.empty())
			continue;
		if (key == "server") {
			server_count++;
			_server.resize(server_count + 1);
			is_acc = false;
			continue;
		}
		if (key == "location" && s1[s1.length() - 1] == '{') {
			is_acc = true;
			map_key = s1.substr(s1.find(' ') + 1, s1.find('{') - s1.find(' ') - 2); // set map_key for the new way
			_server[server_count - 1]._location_map[map_key];
		}
		else if (s1[s1.length() - 1] == '}') {
			is_acc = false;
		}
		if (!is_acc)
			set_values_server(s1, _server[server_count - 1]);
		if (is_acc) {
			// send the map with the appropriate key
			set_values_location(s1, _server[server_count - 1]._location_map[map_key]); // for some reason it is not working yet
		}
	}
}

const std::vector<HTTP::t_server> &HTTP::parse_conf::get_server() const
{
    //the total config file
	return _server;
}

// gaat dit werken?
int HTTP::parse_conf::get_server_port(const t_server &server) {
	return server._port;
}

std::vector<int> 	HTTP::parse_conf::get_ports()
{
	std::vector<int> ports;
	for (unsigned long i = 0; i < _server.size(); i++)
		ports.push_back(get_server_port(_server[i]));
	return (ports);
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

