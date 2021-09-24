#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include "respond.hpp"
#include "../utils/colors.hpp"
#include "../utils/utils.hpp"
#include "../Respond/CGI.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#define DEFAULT_CLIENT_BODY_SIZE 1024*1024

HTTP::respond::respond(t_req_n_config req_n_conf, t_client_select &client, char *&buffer)
{
	make_error_codes_map();
	_status_code = 0;
	_map_req = req_n_conf._req_map;
	_pars_server = req_n_conf._parser_server;

	if (_map_req["PROTOCOL"].compare("HTTP/1.1") != 0)
		set_status_code(405);
	else if (_map_req["METHOD"].compare("GET") == 0)
		getmethod();
	else if (_map_req["METHOD"].compare("POST") == 0)
		postmethod(client, buffer);
	else if (_map_req["METHOD"].compare("DELETE") == 0)
		deletemethod(client);
	else
	{
		set_status_code(405);
		getmethod();
	}
}
/* destructor */
HTTP::respond::~respond() {}

/* copy constructor */
HTTP::respond::respond(const respond &x)
{
	_statusline = x._statusline;
	_contentlen = x._contentlen;
	_lastmodified = x._lastmodified;
	_connection = x._connection;
	_contentype = x._contentype;
	_date = x._date;
	_host = x._host;
	_language = x._language;
	_body = x._body;
	_pars_server = x._pars_server;
	_status_code = x._status_code;
	_totalpath = x._totalpath;
	_relativepath = x._relativepath;
	_totalheader = x._totalheader;
	_totalrespond = x._totalrespond;
	_map_req = x._map_req;
}

/*assignment operator */
HTTP::respond &HTTP::respond::operator=(const respond &x)
{
	_statusline = x._statusline;
	_contentlen = x._contentlen;
	_lastmodified = x._lastmodified;
	_connection = x._connection;
	_contentype = x._contentype;
	_date = x._date;
	_host = x._host;
	_language = x._language;
	_body = x._body;
	_pars_server = x._pars_server;
	_status_code = x._status_code;
	_totalpath = x._totalpath;
	_relativepath = x._relativepath;
	_totalheader = x._totalheader;
	_totalrespond = x._totalrespond;
	_map_req = x._map_req;
	return *this;
}

void HTTP::respond::getmethod()
{
	std::string findKey;
	_body = "";
	find_total_file_path();
	set_date();
	set_modified();
	set_connection(_map_req["Connection:"]);
	set_host(_map_req["Host:"]);
	set_language(_map_req["Accept-Language:"]);
	set_server_name();
	set_body();
	set_status_line();
	set_content_len(_body);
	set_total_response();
}


void HTTP::respond::postmethod(t_client_select &client, char * &buffer)
{
	/* Does path (directory) exist? */
	struct stat stats;
	std::string	directory;
	find_total_file_path();
	directory = _totalpath.substr(0, _totalpath.rfind('/'));
	if (stat(directory.c_str(), &stats) == -1)
	{
		_status_code = 404;
		client._header = _status_errors[404];
		return;
	}

	/* Method POST Allowed? */
	if (_status_code == 405)
	{
		client._header = "HTTP/1.1 405 Method Not Allowed\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
		client._expect_body = false;
		return;
	}

	/* Chunked request? */
	client._chunked = _map_req["Transfer-Encoding:"] == " chunked";
	
	/* If not chunked get Content-Length and check if it is larger then allowed in the config, return 413
	 * (Chunked Request doesn't state a Content-Length) */
	if (!client._chunked)
	{
		client._content_length = ft_stoi(_map_req["Content-Length:"]);
		if (find_client_body_size() < (uint64_t)client._content_length)
		{
			_status_code = 413;
			std::string payload = "Max upload-size: " + ft_numtos(find_client_body_size());
			client._header = "HTTP/1.1 413 Request Entity Too Large\r\nConnection: keep-alive\r\nContent-Length: " + ft_numtos(payload.length() + 2) + "\r\n\r\n" + payload + "\r\n";
			client._expect_body = false;
			client._close_connection = true;
			return;
		}
	}

	/* Create file */
	client._filename = _totalpath;
	std::ofstream out_file(client._filename.c_str(), std::ios::binary);

	/* Check if POST contains body, if not send 100 Continue */
	if (!(_map_req.count("Expect:")))
	{
		std::string tmp_body(buffer);

		if (client._chunked)
		{
			std::string tmp_first_line(buffer);
			/* get rid of header and empty space (\r\n\r\n) */
			tmp_first_line = tmp_first_line.substr(tmp_first_line.find("\r\n\r\n") + 4);
			/* we now can set this as the body. The closing 0 (end of chunked-request) is
			 * included but we can ignore this since we know the length to write */
			tmp_body = tmp_first_line.substr(tmp_first_line.find("\r\n") + 2);
			/* trim the first line to be able to read the bytes we need to write */
			tmp_first_line = tmp_first_line.substr(0, tmp_first_line.find("\r\n"));
			//client._content_length = hex2int((char *)tmp_first_line.c_str());
			client._content_length = hex2int(tmp_first_line);
			/* write to file and return */
			out_file.write(tmp_body.c_str(), client._content_length);
			out_file.close();
			client._expect_body = false;
			client._post_done = true;
			client._header = "HTTP/1.1 204 No Content\r\n\r\n";

			return;
		}

		tmp_body = tmp_body.substr(tmp_body.find("\r\n\r\n") + 4);

		out_file.write(tmp_body.c_str(), client._content_length);
		out_file.close();
		client._expect_body = false;
		client._post_done = true;
		client._header = "HTTP/1.1 204 No Content\r\n\r\n";
		return ;
	}
	else
	{
		out_file.close();
		client._header = "HTTP/1.1 100 Continue\r\n\r\n";
		client._total_body_length = 0;
		client._expect_body = true;
		client._post_done= false;
		return;
	}
}

/*
 * Retrieve max client body size from config file
 *
 * Workings:
 * location /uploads/
 * 		client_body_size 1000
 *
 * 	location /uploads/new
 * 		client_body_size inherited from /uploads/ unless stated otherwise
 */
uint64_t HTTP::respond::find_client_body_size()
{
	std::string	path;
	size_t 		found_slash;

	found_slash = _map_req["URI"].rfind('/');
	if (found_slash == std::string::npos)
		path = "/";
	else
	{
		std::string tmp = _map_req["URI"].substr(0, found_slash + 1);
		path = "/" + tmp;
	}
	while (true)
	{
		if (_pars_server._location_map.count(path) &&
			_pars_server._location_map[path]._client_body_size)
		{
			return (_pars_server._location_map[path]._client_body_size);
		}
		else if (path.length() > 1)
		{
			path = path.substr(0, path.rfind('/'));
			path = path.substr(0, path.rfind('/') + 1);
		}
		else
			return (DEFAULT_CLIENT_BODY_SIZE);
	}
}

void HTTP::respond::post_response(t_client_select &client)
{
	client._header = "HTTP/1.1 204 No Content\r\n\r\n";
}

void HTTP::respond::deletemethod(t_client_select &client)
{
	(void)client;
	find_total_file_path();
	_body = "";
	if (_status_code != 405)
	{
		char filename[_totalpath.size() + 1];
		strcpy(filename,_totalpath.c_str());
		/* remove returns -1 on failure */
		int result = remove(filename);
		if (result == -1)
			_status_code = 404;
		else
		{
			_status_code = 204;
			_totalheader = "HTTP/1.1 204 No Content\r\n\r\n";
			return;
		}
	}
	set_status_line();
	set_content_len(_body);
	set_date();
	set_host(_map_req["Host:"]);
	set_server_name();
	set_total_response();
}

void HTTP::respond::set_no_config()
{
	_statusline.append(_stat_cha);
	_body.append(_stat_cha);
	_body = "<h1>status code is not present in config file</h1>\0";
	return;
}

void HTTP::respond::make_error_codes_map()
{
	_status_errors[404] = "<html><h1>404: You can't do that!</h1></html>";
	_status_errors[403] = "<html><h1>403: You can't do that!</h1></html>";
	_status_errors[405] = "<html><h1>405: Try another method!</h1></html>";
	_status_errors[301] = "<html><h1>405::Bad Request The request could not be understood by the \
    server due to malformed syntax. The client SHOULD NOT repeat the request without modifications.</h1></html>";
	_status_errors[413] = "<html><h1> 400 : Request Entity Too Large</h1></html>";
	_status_errors[500] = "<html><h1>500 : Internal Server Error</h1><div class=\"tenor-gif-embed\" data-postid=\"22864502\" data-share-method=\"host\" data-aspect-ratio=\"1\" data-width=\"25%\"><a href=\"https://tenor.com/view/sorry-tony-hayward-south-park-s14e11-coon2rise-of-captain-hindsight-gif-22864502\">Sorry Tony Hayward GIF</a>from <a href=\"https://tenor.com/search/sorry-gifs\">Sorry GIFs</a></div> <script type=\"text/javascript\" async src=\"https://tenor.com/embed.js\"></script></html>";
}

void	HTTP::respond::reset_body_error()
{
	if (_status_code == 200 || _status_code == 204)
		return;
	if (_pars_server._error_page.size() > 1)
	{
		std::string char_status_code = ft_numtos(_status_code);
		if (_pars_server._error_page[0] == char_status_code)
		{
			std::string rel_err_pg = _pars_server._error_page[1];
			std::string err_pg = _root.append(_pars_server._error_page[1]);
			std::ifstream file(err_pg.c_str());
			if (file.is_open())
			{
				_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
			}
			else
			{
				_status_code = 403;
				_body = _status_errors[403];
			}
		}
	}
}

void HTTP::respond::set_status_line()
{
	std::string tmp = ft_numtos(_status_code);
	_stat_cha = tmp.c_str();
	std::string total_body;
	std::string root;
	if (_pars_server._error_page.size() > 1)
		root = _pars_server._error_page[1];
	else
		root = "error_page.html";
	std::ifstream file("html_pages/auto_error.html");
	std::string _stat_cha_s = _stat_cha;
	_stat_cha_s.append(";");;
	if (_pars_server._error_page[0] == _stat_cha || _pars_server._error_page[0] == _stat_cha_s )
		set_no_config();
	if (_status_code == 404)
	{
		_statusline = "HTTP/1.1 404 Not Found";
		_body = _status_errors[404];
	}
	else if (_status_code == 200)
	{
		_statusline = "HTTP/1.1 200 OK";
		return;
	}
	else if (_status_code == 403)
	{
		_statusline = "HTTP/1.1 403 Forbidden";
		_body = _status_errors[403];
	}
	else if (_status_code == 405)
	{
		_statusline = "HTTP/1.1 405 Method not Allowed";
		_body = _status_errors[405];
	}
	else if (_status_code == 204)
		_statusline = "HTTP/1.1 204 No Content";
	else if (_status_code == 301)
		_statusline = "HTTP/1.1 301 Moved Permanently";
	else if(_status_code == 413)
	{
		_statusline.append("HTTP/1.1 413 Payload Too Large");
		_body = _status_errors[413];
	}
	else if(_status_code == 500)
	{
		_statusline.append("HTTP/1.1 505 Internal Server Error");
		_body = _status_errors[500];

	}
	if(file.is_open())
	{
		total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_body = total_body;
		file.close();
	}
	reset_body_error();
	set_content_type("text/html");
}

void HTTP::respond::set_content_type(const std::string &contentype)
{
	_contentype = contentype;
	_totalrespond.insert(std::pair<std::string, std::string>("Content-Type", _contentype));
}

void HTTP::respond::set_date()
{
	struct timeval tv;
	time_t t;
	struct tm *info;
	char buffer[164];
	gettimeofday(&tv, NULL);
	t = tv.tv_sec;
	info = localtime(&t);
	strftime(buffer, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
	_date = buffer;
	_totalrespond.insert(std::pair<std::string, std::string>("Date", _date));
}

void HTTP::respond::set_server_name()
{
	_servername = _pars_server._server_name;
	_totalrespond.insert(std::pair<std::string, std::string>("Server", _servername));
}

void ::HTTP::respond::set_modified()
{
	struct stat stats;
	struct tm *info;
	char timestamp[1000];

	if (stat(_totalpath.c_str(), &stats) == 0)
	{
		info = gmtime(&stats.st_mtime);
		strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
		_lastmodified = timestamp;

	}
	_totalrespond.insert(std::pair<std::string, std::string>("Last-Modified", _lastmodified));
}

void HTTP::respond::set_connection(std::string connection)
{
	if (connection.empty())
		return;
	_connection = connection;
	_connection = _connection.substr(1, _connection.size() - 1);
	_totalrespond.insert(std::pair<std::string, std::string>("Connection", _connection));
}

void HTTP::respond::set_host(std::string host)
{
	std::string host_cmp;

	host = host.substr(1, host.size() - 1);
	host_cmp = host.substr(0, host.find(':'));
	_host = host;
	_totalrespond.insert(std::pair<std::string, std::string>("Host", _host));
}

void HTTP::respond::set_language(std::string contentlanguage)
{
	_language = contentlanguage;
	_totalrespond.insert(std::pair<std::string, std::string>("Content-Language", _language));
}

void HTTP::respond::set_content_len(std::string body)
{
	int size;

	size = body.size();
	std::stringstream ss;
	ss << size;
	ss >> _contentlen;
	_totalrespond.insert(std::pair<std::string, std::string>("Content-Length", _contentlen));
}

void HTTP::respond::set_total_response()
{
	std::map<std::string, std::string>::iterator it;
	_totalheader.append(_statusline);
	_totalheader.append("\r\n");
	it = _totalrespond.begin();
	for (it = _totalrespond.begin(); it != _totalrespond.end(); ++it)
	{
		if (!it->second.empty())
		{
			_totalheader.append(it->first);
			_totalheader.append(": ");
			_totalheader.append(it->second);
			_totalheader.append("\r\n");
		}
	}
	_totalheader.append("\r\n");
	_totalheader.append(_body);
	_totalheader.append("\r\n");
}

void HTTP::respond::find_total_file_path()
{
	std::string key;
	std::string redir;
	int found;

	found = -1;
	_relativepath = _map_req["URI"];
	if (_relativepath[_relativepath.size() - 1] != '/')
		_relativepath = _relativepath.append("/");
	key = "/";
	key = key.append(_relativepath);
	while (key != "/")
	{
		key = key.substr(0, key.find_last_of('/') + 1);
		if (_pars_server._location_map.count(key) == 1)
			break;
		if (key[key.size() - 1] == '/')
			key = key.substr(0, key.size() - 1);
	}
	if (_pars_server._location_map[key]._redir != "")
	{
		_status_code = 301;
		_relativepath = _relativepath.substr(key.size() - 1, _relativepath.size() - key.size() + 1);
		redir = _pars_server._location_map[key]._redir;
		_relativepath = _pars_server._location_map[key]._redir.append(_relativepath);
		if (_relativepath[_relativepath.size()] == '/')
			_relativepath = _relativepath.substr(0, _relativepath.size() - 1);
		key = redir;
	}
	if (_relativepath[_relativepath.size() - 1] == '/')
		_relativepath = _relativepath.substr(0, _relativepath.size() - 1);
	if (_pars_server._location_map.count(key) == 1)
	{
		for (unsigned int i = 0; i < _pars_server._location_map[key]._methods.size(); i++)
		{
			if (_map_req["METHOD"].compare(_pars_server._location_map[key]._methods[i]) == 0)
				found = 1;
		}
		if (found == -1)
			_status_code = 405;
		if (_relativepath == "" || _relativepath == "/")
			_relativepath = _pars_server._location_map[key]._index;
		if (_pars_server._location_map[key]._root.empty())
		{
			_root = _pars_server._location_map["/"]._root;
			_totalpath = _pars_server._location_map["/"]._root.append(_relativepath);
		}
		else
		{
			_root = _pars_server._location_map[key]._root;
			_totalpath = _pars_server._location_map[key]._root.append(_relativepath);
		}
	}
	return;
}

void HTTP::respond::set_status_code(int code)
{
	_status_code = code;
	return;
}

void HTTP::respond::set_body()
{
	const char *_path;
	struct stat sb;

	_path = _totalpath.c_str();
	if (_status_code == 405)
		return;
	if (_pars_server._auto_index == 1)
	{
		if (opendir(_path) != NULL)
		{
			std::ifstream file("bin/auto_index.php");
			if (file.is_open())
			{
				HTTP::CGI cgi(_map_req, _pars_server, "bin/auto_index.php", _root);
				_body = cgi.get_cgi_body();
				_status_code = 200;
				file.close();
				return;
			}
		}
	}
	if (stat(_path, &sb) == -1)
		return (set_status_code(404));
	if (_totalpath.find(".php") != std::string::npos)
	{
		HTTP::CGI cgi(_map_req, _pars_server, _totalpath, _root);
		_status_code = cgi.get_status_code();
		if (_status_code != 500) {
			_body = cgi.get_cgi_body();
		}
		else
			return (set_status_code(500));
	}
	else
	{
		std::ifstream file(_path, std::ios::in | std::ios_base::binary);
		if (file.is_open())
			_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		else
			return (set_status_code(403));
		file.close();
	}
	if (_status_code == 0)
		_status_code = 200;
}

const std::string &HTTP::respond::getTotalheader() const
{
	return _totalheader;
}
