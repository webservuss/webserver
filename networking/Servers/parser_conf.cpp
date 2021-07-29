//
// Created by ruben on 13-07-21.
//

#include "parser_conf.hpp"

void parse_conf::set_values_server(std::string s, t_server &server)
{
	// this probably needs a better name than 'key'
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
		server._root = s.substr(s.find(' ') + 1, s.size());
	if (key == "index")
		server._index = s.substr(s.find(' ') + 1, s.size());
	if (key == "key")
		server._key = s.substr(s.find(' ') + 1, s.size());
	if (key == "value")
		server._value = s.substr(s.find(' ') + 1, s.size());
}


void	parse_conf::set_values_location(std::string s, t_location &location)
{

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


int parse_conf::get_server_port(const t_server &server) {
	return server._port;
}
