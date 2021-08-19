#include "parser_conf.hpp"

HTTP::parse_conf::parse_conf(const char *path)
{
	std::string 	line;
	bool			is_acc;
	int				server_count;
	std::string		map_key;
	std::string 	s1;
	int 			i;
    std::ifstream	file;
    file.open(path);

	is_acc = false;
	server_count = 0;
    if(!file || !file.is_open())
	{
        std::cout << " ERROR no configfile" << std::endl;
		exit(1);
	}
	while(std::getline(file, line, '\n'))
	{
		i = 0;
		while (line[i] == '\t' || line[i] == '\n')
			i++;
		s1 = line.substr(i, line.size() - i);
		std::string key = s1.substr(0, s1.find(" "));
		s1 = s1.substr(0, s1.find('\n'));
		if (s1.empty())
			continue;
		if (key == "server")
		{
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
		if (!is_acc)
			set_values_server(s1, _server[server_count - 1]);
		if (is_acc)
			set_values_location(s1, _server[server_count - 1]._location_map[map_key]);
	}
    file.close(); // put in destructor
}

// CAN SOMEONE CHECK COPY CONSTRUCTOR AND ASSIGNMENT OPPERATOR
/*copy constructor */
HTTP::parse_conf::parse_conf(const parse_conf& x)
{ // TEST THIS // change
    _server = x._server;
}

HTTP::parse_conf::~parse_conf() {}

HTTP::parse_conf& HTTP::parse_conf::operator=(const parse_conf& x)
{
    _server = x._server;
	return *this;
}

void HTTP::parse_conf::set_values_server(std::string s, t_server &server)
{
	std::string key = s.substr(0, s.find(' '));
	std::string value = s.substr(s.find(' ') + 1, s.length() - s.find(' ') - 2);
	if (key == "server_name")
		server._server_name = value;
	if (key == "listen")
		server._port = ft_stoi((s.substr(s.find(' ') + 1, s.size())));
	if (key == "host")
		server._host = value;
	if (key == "error_page")
		server._error_page = split(s.substr(s.find(' ') + 1, s.size()), ' ');
	if (key == "root")
		server._root = value;
	if (key == "index")
		server._index = value;
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
}

// This function gets the whole line and a struct (t_location)
void	HTTP::parse_conf::set_values_location(std::string s, t_location &location)
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
	if (key == "index")
		location._index= value;
	if (key == "client_body_size")
		location._client_body_size = ft_stoi(value);
	if (key == "return")
	{	
		if (value.substr(0, value.find(' ')) == "301")
		{
			value = value.substr(value.find_last_of(' ') + 1, value.size() - value.find_last_of(' '));
			location._redir = value;
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
