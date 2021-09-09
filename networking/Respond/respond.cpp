#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/time.h>
#include <stdlib.h>
#include "respond.hpp"
#include "../utils/colors.hpp"
#include "../utils/utils.hpp"
#include "../Respond/CGI.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>

#define DEFAULT_CLIENT_BODY_SIZE 1024*1024

HTTP::respond::respond(t_req_n_config req_n_conf, t_client_select &client, char * &buffer, int valread)
{
    _status_code = 0;
    _map_req = req_n_conf._req_map;
    _pars_server = req_n_conf._parser_server;

    if (_map_req["PROTOCOL"].compare("HTTP/1.1") != 0)
        set_status_code(405);
    else if (_map_req["METHOD"].compare("GET") == 0)
        getmethod();
     else if (_map_req["METHOD"].compare("POST") == 0)
         postmethod(client, buffer, valread);
     else if (_map_req["METHOD"].compare("DELETE") == 0)
         deletemethod();
    else
        set_status_code(405);
}

HTTP::respond::~respond()   {}


HTTP::respond::respond(const respond& x)
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
HTTP::respond& HTTP::respond::operator=(const respond& x)
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

    find_total_file_path();
    set_date();
    set_modified();
    set_connection(_map_req["Connection:"]);
    set_host(_map_req["Host:"]);
    set_language(_map_req["Accept-Language:"]);
    set_server_name();
    set_body();
    set_status_line();
    set_total_response();
}

//static void make_directory(std::string dir)
//{
//
//}

void HTTP::respond::postmethod(t_client_select &client, char * &buffer, int valread)
{

	// TODO: is this enough?
	client._filename = "www/html_pages/uploads/" + _map_req["URI"];
//	struct stat stat_buffer;

//	std::cout << stat("www/html_pageploads/", &stat_buffer) << std::endl;
//	mkdir("/Users/rvan-sch/webservuss_7sep/www/html_pages/uploads/test/test2", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//	exit(1);
	std::ofstream out_file(client._filename.c_str(), std::ios::binary);
	client._content_length = ft_stoi(_map_req["Content-Length:"]);


	// TODO: request method invalid? */
	find_total_file_path();
	if (_status_code == 405) {
		client._header = "HTTP/1.1 405 Method Not Allowed\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
		client._expect_body = false;
		return;
	}

	// TODO: client body size valid?
	if (find_client_body_size() < (uint64_t)client._content_length)
		_status_code = 413;


	if (_status_code == 413) {
		//client._header = "HTTP/1.1 413 Request Entity Too Large\r\nConnection: keep-alive\r\nContent-Length: 334\r\n\r\n<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Plebbin reeee</title></head><body style='background-color: #f72d49; padding: 50px 10vw 0 10vw; color: #3f3f3f;'><h1>Error: 413</h1><p style='size: 15px;'>Request Entity Too Large</p></body></html>\r\n";
		std::string payload = "Max upload-size: " + ft_numtos(find_client_body_size());

		client._header = "HTTP/1.1 413 Request Entity Too Large\r\nConnection: keep-alive\r\nContent-Length: " + ft_numtos(payload.length() + 2) + "\r\n\r\n" + payload + "\r\n";

		client._expect_body = false;
		return;
	}


	if (!(_map_req.count("Expect:")))
	{
		std::string tmp(buffer);
		int position_of_body = tmp.find("\r\n\r\n") + 4;
		// TODO: check if length of body is comp with Content-Length
		out_file.write(&buffer[position_of_body], client._content_length);
		out_file.close();
		client._expect_body = false;
		client._post_done = true;

		(void)valread;


		client._header = "HTTP/1.1 204 No Content\r\n\r\n";
		//HTTP::respond::post_response(client, client._content_length, body);
		//return 0;
		return ;
	}
	else
	{
		out_file.close();
		/* send a brief response to the client */
		client._header = "HTTP/1.1 100 Continue\r\n\r\n";
		client._total_body_length = 0;
		client._expect_body = true;
		client._post_done= false;
		//return 1;
		return;
	}
}

uint64_t HTTP::respond::find_client_body_size()
{
	/*
	 * location /uploads/
	 * 		client_body_size 1000
	 *
	 * 	location /uploads/new
	 * 		client_body_size inherited from /uploads/ unless stated otherwise
	 */


	std::string path;
	size_t found_slash = _map_req["URI"].rfind('/');
	if (found_slash == std::string::npos) {
		path = "/";
	}
	else
	{
		std::string tmp = _map_req["URI"].substr(0, found_slash+1);
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
		{
			return (DEFAULT_CLIENT_BODY_SIZE);
		}
	}
}

/*
void HTTP::respond::post_handle_request(t_client_select &client, char * &buffer, int valread)
{
	(void)valread;
	// TODO directory has to be taken from config file? Also: ofstream does not create a directory
	client._filename = "www/html_pages/uploads/" + .r_n_c._req_map["URI"]; // relative path of the server executable (don't start with a '/' !)
	std::ofstream out_file(client._filename.c_str(), std::ios::binary);
	client._content_length = ft_stoi(r_n_c._req_map["Content-Length:"]);
	// TODO: request method valid?

	// TODO: client body size valid?

	if (!(r_n_c._req_map.count("Expect:")))
	{
		int position_of_body = stringbuff.find("\r\n\r\n") + 4;
		out_file.write(&buffer[position_of_body], client._content_length);
		out_file.close();
		client._expect_body = false;
		client._post_done = true;
		std::string body(&buffer[position_of_body]);

		HTTP::respond::post_response(client, client._content_length, body);
		return 0;
	}
	else
	{
		out_file.close();
		// send a brief response to the client
		client._header = "HTTP/1.1 100 Continue\r\n\r\n";
		client._expect_body = true;
		client._post_done= false;
		return 1;
	}
}
*/

void HTTP::respond::post_response(t_client_select &client, const int &total_body_length, std::string &body)
{

	(void)body;
	client._header = "HTTP/1.1 204 No Content\r\n\r\n";


//	client._header = "HTTP/1.1 200 OK\r\nContent-Type: text/markdown\r\nContent-Length: " + ft_numtos(total_body_length) + "\r\nabc\r\n";



	(void)total_body_length;
//	client._header = "HTTP/1.1 200 OK\r\n";
//	client._header.append("Content-Length: " + ft_numtos(total_body_length) + "\r\n");
//	client._header.append("Connection: keep-alive\r\n");
//	client._header.append("Location: /uploads/a.txt\r\n");
//	client._header.append("Content-Type: text/html; charset=utf-8\r\n");
//	client._header.append("Host: localhost:8080\r\n");
//	client._header.append( "Date: " +d + "\r\n");
//	client._header.append("Server: localhost\r\n");
//	client._header.append("\r\n");
//
//	client._header.append(body + "\r\n");
////	client._header.append("\r\n");
//	char a[2];
//	a[0]='a';
//	a[1]=0;
//	std::string b(a);
//	client._header.append(b+"\r\n");
//	client._header.append("\r\n");
//
}



void HTTP::respond::deletemethod()
{
    // _postheader = _totalheader;
    // int ret  = remove(file.c_str());
    // if( ret != 0)
    // set statuscode notfound

}

void HTTP::respond::set_no_config(std::string root)
{
    std::cout << "no config ?" << std::endl;
    if(root != "error_page.html;" )
    {   
        std::cout << YELLOW << "THER IS NO CONFIG" << R << std::endl;
        _statusline.append(_stat_cha);
        _body.append(_stat_cha);
        _body = "<h1>status code is not present in config file</h1>\0";
        std::cout << _body  << "BODY"<< std::endl;
        set_content_len(_body);
    }
    return ;
}


void HTTP::respond::set_status_line()
{
    std::string tmp = ft_numtos(_status_code);
    _stat_cha = tmp.c_str();
    std::string total_body;
    std::string root = _pars_server._error_page[1];
    std::ifstream file("html_pages/auto_error.html");
    _statusline = "HTTP/1.1 ";
    std::string _stat_cha_s = _stat_cha;
    _stat_cha_s.append(";");
    std::cout <<GREEN << "status code: " << _status_code << RESET<< std::endl;
    if ( _pars_server._error_page[0] == _stat_cha || _pars_server._error_page[0] == _stat_cha_s )
    {
         set_no_config(root);
         std::cout << "out of config" << std::endl;
    }

    if (_status_code == 404)
    {
         _statusline.append("404 Not Found");
        _body = "<html><h1>404: You can't do that!</h1></html>";
    }
    else if (_status_code == 200)
    {
        _statusline.append("200 OK");
        return;
    }
    else if (_status_code == 403)
    {
        _statusline.append("403 Forbidden");
        _body = "<html><h1>403: You can't do that!</h1></html>";
    }
    else if (_status_code == 405)
    {
        _statusline.append("405 Method not Allowed");
        _body = "<html><h1>405: Try another method!</h1></html>";
    }
    else if (_status_code == 204)
    {
        _statusline.append("204 No Content");
        // _body = "<html><h1>204: no content</h1></html>";
    }
    else if (_status_code == 301)
        _statusline.append("301 Moved Permanently");
    else if(_status_code == 301)
    {
        _statusline.append("4 ");
        _body = "<html><h1> 4 :Bad Request The request could not be understood by the server due to malformed syntax. The client SHOULD NOT repeat the request without modifications. </h1></html>";
    }
    else if(_status_code == 413)
    {
        _statusline.append("413 ");
        _body = "<html><h1> 400 : Request Entity Too Large</h1></html>";
    }
    if(file.is_open())
        {
            std::cout << "file open " << std::endl;
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
        }
    file.close();
    set_content_type("text/html");
    set_content_len(_body);
   

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
    //_date = strftime(buffer, sizeof buffer, "%a, %d %B %Y %H::%M::%S %Z", info);
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
    _connection = connection;
	_connection = _connection.substr(1, _connection.size() - 1);
    _totalrespond.insert(std::pair<std::string, std::string>("Connection", _connection));
}

void HTTP::respond::set_host(std::string host)
{
    std::string host_cmp;

    host = host.substr(1, host.size() - 1);
    host_cmp = host.substr(0, host.find(':'));
    // TODO get rid of this
//    if (host_cmp != _pars_server._host)
//        return (set_status_code(400));
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
    _relativepath = _map_req["URI"].c_str();
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
        _relativepath = _relativepath.substr(0, _relativepath.size() -1);
    if (_pars_server._location_map.count(key) == 1)
    {
		for (unsigned int i = 0; i < _pars_server._location_map[key]._methods.size(); i++)
		{
            if (_map_req["METHOD"].compare(_pars_server._location_map[key]._methods[i]) == 0)
                found = 1;
		}
        if (found == -1)
            return (set_status_code(405));
        if (_relativepath == "" || _relativepath == "/")
            _relativepath = _pars_server._location_map[key]._index;
        _totalpath = _pars_server._location_map[key]._root.append(_relativepath);
    }
    return ;
}

void HTTP::respond::set_status_code(int code)
{
    _status_code = code;
    return;
}

void HTTP::respond::set_body()
{
    const char      *_path;
    struct stat     sb;

    _path = _totalpath.c_str();
    if (_status_code == 405)
        return;
    if (_pars_server._auto_index == 1)
    {
        if (opendir(_path) != NULL)
        {
                std::ifstream file("www/html_pages/downloads/index.php");
                if (file.is_open())
                {
                    _totalpath = "www/html_pages/downloads/index.php";
                    HTTP::CGI cgi(_map_req, _pars_server, _totalpath);
		            _body = cgi.get_cgi_body();
                    set_content_len(_body);
                    _status_code = 200;
                    file.close();
                    return ;
                }
        }
    }
	if (stat(_path, &sb) == -1)
		return (set_status_code(404)); // file doesnt exist
	if (_totalpath.find(".php") != std::string::npos)// _body will be filled by php_cgi()
	{
		HTTP::CGI cgi(_map_req, _pars_server, _totalpath);
		_body = cgi.get_cgi_body();
	}
	else
	{
        std::ifstream   file(_path);
		if(file.is_open())
            _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        else
			return (set_status_code(403));
		file.close();
	}
    set_content_len(_body);
   //if(status_code = 204 )
    // if (_contentlen == "0" && _status_code == 0)
    // {
    //     _status_code = 204;
    // }
    if (_status_code == 0)
        _status_code = 200;

}

const std::string &HTTP::respond::getTotalheader() const
{
    return _totalheader;
}
