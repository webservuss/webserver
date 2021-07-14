//
// Created by ruben on 13-07-21.
//

#include "parser_conf.hpp"

int g_amount = 0;

static int ft_stoi(std::string s)
{
	int i;
	std::istringstream(s) >> i;
	return i;
}

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string/39359311
std::vector<std::string> parse_conf::split(const std::string &s, char delim)
{
	std::vector<std::string>	elems;
	std::stringstream			ss(s);
	std::string 				item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

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

void dbe(std::string &s)
{
	std::cout << s << std::endl;
}
void db(std::string &s)
{
	std::cout << s;
}

void parse_conf::set_values_location(std::string s, int i)
{
    int size;
	std::string key = s.substr(0, s.find(' '));
	if (key == "method")
		_location[i-1]._method = s.substr(s.find(' ') + 1, s.size());


	if (key == "location") {
		s = s.substr(s.find(' '),  s.size());
		std::cout<< "*********s" << s << "*******"<< std::endl;
		size = s.find ('{' );
        s = s.substr(0 ,  size);
		_location[i-1]._address = s;
	}





	if (key == "cgi")
		_location[i-1]._cgi = s.substr(s.find(' ') + 1, s.size());
	if (key == "root")
		_location[i-1]._root = s.substr(s.find(' ') + 1, s.size());
	if (key == "autoindex")
		_location[i-1]._autoindex= s.substr(s.find(' ') + 1, s.size());
	if (key == "client_body_size")
		_location[i-1]._client_body_size = ft_stoi(s.substr(s.find(' ') + 1, s.size()));
	//std::cout << "l[0]: " << _location[0]._method << std::endl;

}

int	count_locations(std::ifstream &file)
{
	std::string line;
	while(std::getline(file, line, '\t')) {
		std::string key = line.substr(0, line.find(" "));
		if (key == "location")
			g_amount++;
	}
	return g_amount;

}

	parse_conf::parse_conf(std::ifstream &file)
{
	if (!file) {
		perror("No file");
		exit(1);
	}
	std::string line;
	// conf must have empty line at end?
	int i =0;
	bool is_acc = false;
	//int amount_loc = count_locations(file);
	//std::cout << "amount_loc: " << amount_loc << std::endl;
	int vector_size = 0;
	while(std::getline(file, line, '\t'))
	{
		std::string key = line.substr(0, line.find(" "));
		line = line.substr(0, line.find('\n'));
		if (line.empty())
			continue;
		if (line[line.length() - 1] == ';') {
			line = line.substr(0, line.find(';'));
		}
		else if (key == "location" && line[line.length() - 1] == '{') {
			is_acc = true;
			vector_size++;
			_location.resize(vector_size);
		}
		else if (line[line.length() - 1] == '}') {
			is_acc = false;
		}

		std::cout << line << std::endl;
		// prob. needs a better name than key
		if (!is_acc)
			set_values_server(line);
		if (is_acc)
			set_values_location(line, vector_size);


		//if(line.find(s) != std::string::npos) {
		//	_s = line.substr(line.find(' ') + 1, line.size());
			//_server_name = line.substr(line.find(" "), line.find(";"));




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

const std::vector<t_location> &parse_conf::get_location() const
{
	return _location;
}
