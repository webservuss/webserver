#include "parser_conf.hpp"

void HTTP::parse_conf::set_values_server(std::string s, t_server &server)
{
	std::string key = s.substr(0, s.find(' '));
	std::string value = s.substr(s.find(' ') + 1, s.length() - s.find(' ') - 2);
	if (key == "server_name")
		server._server_name = s.substr(s.find(' ') + 1, s.size());
	if (key == "listen")
		server._port = ft_stoi((s.substr(s.find(' ') + 1, s.size())));
	if (key == "host")
		server._host = s.substr(s.find(' ') + 1, s.size());
	if (key == "error_page")
		server._error_page = split(s.substr(s.find(' ') + 1, s.size()), ' ');
	if (key == "autoindex")
	{
		std::cout << " >> " << value << std::endl;
		if (value == "on")
			server._auto_index = 1;
		else if (value == "off")
			server._auto_index = 0;
		else {
			perror("Config file: auto_index incorrect value");
			exit(1);
		}
	}
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

	std::string key = s.substr(0, s.find(' '));
	std::string value = s.substr(s.find(' ') + 1,s.find(';') - s.find(' ') - 1);
	if (key == "method") {
		std::cout << "...." << std::endl;
		location._method= value;
	}
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
	bool		is_acc = false;
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
			_server.resize(server_count);
			is_acc = false;
			continue;
		}
		if (key == "location" && s1[s1.length() - 1] == '{') {
			is_acc = true;
			map_key = s1.substr(s1.find(' ') + 1, s1.find('{') - s1.find(' ') - 2);
			_server[server_count - 1]._location_map[map_key];
			continue;
		}
		else if (s1[s1.length() - 1] == '}') {
			is_acc = false;
			continue;
		}
		std::cout << line << std::endl;
		if (!is_acc)
			set_values_server(s1, _server[server_count - 1]);
		if (is_acc) {
			set_values_location(s1, _server[server_count - 1]._location_map[map_key]);
			std::cout << "map_key: " << map_key << std::endl;
			std::cout << _server[server_count - 1]._location_map[map_key]._method << std::endl;
		}
	}
}

const std::vector<HTTP::t_server> &HTTP::parse_conf::get_server() const
{
	return _server;
}

int HTTP::parse_conf::get_server_port(const t_server &server)
{
	return server._port;
}

std::vector<int> 	HTTP::parse_conf::get_ports()
{
	std::vector<int> ports;
	for (unsigned long i = 0; i < _server.size(); i++)
		ports.push_back(get_server_port(_server[i]));
	return (ports);
}
