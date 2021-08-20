#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <stdlib.h>
#include "respond.hpp"
#include "../utils/colors.hpp"
#include "../Respond/CGI.hpp"
#include <sys/types.h>
#include <dirent.h>

// TODO : 413 (request entity is larger than limits defined by server

HTTP::respond::respond(t_req_n_config req_n_conf)
{
    _status_code = 0;
    _map_req = req_n_conf._req_map;
    _pars_server = req_n_conf._parser_server;

    if (_map_req["PROTOCOL"].compare("HTTP/1.1") != 0)
        set_status_code(405);
    else if (_map_req["METHOD"].compare("GET") == 0)
        getmethod();
    else if (_map_req["METHOD"].compare("POST") == 0)
        postmethod();
    else if (_map_req["METHOD"].compare("DELETE") == 0)
        deletemethod();
    else
        set_status_code(405);
}

HTTP::respond::~respond()   {}

// CAN SOMEONE CHECK COPY CONSTRUCTOR AND ASSIGNMENT OPPERATOR
/*copy constructor */
HTTP::respond::respond(const respond& x)
{ // TEST THIS // change
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
    filefd = x.filefd; // think this will go
}

/*assignment operator */
HTTP::respond& HTTP::respond::operator=(const respond& x)
{ // TEST THIS
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
    filefd = x.filefd; // think this will go
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

void HTTP::respond::postmethod()
{
    std::cout << "ik ben in en post method" << std::endl;
//    //    int serverMaximum = _body.size();
//    //    if( serverMaximum > _body.length())
//    //        std::cout << " TO BIG MAXIMUM SIZE REACHED" << std::endl;
//    //if (maxbodysize < _body.length()[]
//    //  int fd;
//
//    std::string     total_path = _totalpath;
//    std::ifstream file("html_pages/welcome.php");
//    std::cout << GREEN << "file :: " << file << R << std::endl;
//   // std::ifstream file("html_pages/index.html");
//    //fd = open(&file[0], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
//       // std::ifstream file("html_pages/auto_error.html");
//        std::string total_body;
//        if (file.is_open())
//        {
//            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//            _body = total_body;
//            std::cout << RED <<  "POST =" << _body << R <<  std::endl;
//        }
//    if (this->filefd == -1 && _status_code == 200)
//        // this->setstatus(403);
//        std::cout << "status code 403 " << std::endl;
//    //struct stat statBuf;
//   // if (stat(file, &statBuf) < 0 && _status == 200)
//      //  std::cout << "status code 201 " << std::endl;
//    // this->setstatus(201);
//    std::cout << RED << "BODY =" <<_body << R <<std::endl;
//    if (write(filefd, _body.c_str(),_body.length()) == -1)
//        std::cout<< "WRITE " << std::endl;
//    close(filefd);
//    std::cout << GREEN << "BEN JE HIER  " << file << R << std::endl;
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
    std::string tmp = std::to_string(_status_code);
    _stat_cha = tmp.c_str();
    std::string total_body;
    std::string root = _pars_server._error_page[1];
    std::ifstream file("html_pages/auto_error.html");
    _statusline = "HTTP/1.1 ";
    std::string _stat_cha_s = _stat_cha;
    _stat_cha_s.append(";");
    std::cout <<"status code: " << _status_code << std::endl;
    if ( _pars_server._error_page[0] == _stat_cha || _pars_server._error_page[0] == _stat_cha_s )
    {
         set_no_config(root);
         std::cout << "out of config" << std::endl;
    }

    if (_status_code == 404)
    {
        std::cout << "404" << std::endl;
         _statusline.append("404 Not Found");
        _body = "<h1>404: You can't do that!</h1>\0";
    }
    else if (_status_code == 200)
    {
        _statusline.append("200 OK");
        return;
    }
    else if (_status_code == 403)
    {
        _statusline.append("403 Forbidden");
        _body = "<h1>403: You can't do that!</h1>\0";
    }
    else if (_status_code == 405)
    {
        _statusline.append("405 Method not Allowed");
        _body = "<h1>405: Try another method!</h1>\0";
    }
    else if (_status_code == 204)
    {
        _statusline.append("204 No Content");
        _body = "<h1>204: no content</h1>\0";
    }
    else if (_status_code == 301)
        _statusline.append("301 Moved Permanently");
    else if(_status_code == 301)
    {
        _statusline.append("4 ");
        _body = "<h1> 4 :Bad Request The request could not be understood by the server due to malformed syntax. The client SHOULD NOT repeat the request without modifications. </h1> \0";
    }
    else if(_status_code == 413)
    {
        _statusline.append("113 ");
        _body = "<h1> 400 : Request Entity Too Large \0";
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
    char buffer[64];

    gettimeofday(&tv, NULL);
    t = tv.tv_sec;
    info = localtime(&t);
    _date = strftime(buffer, sizeof buffer, "%a, %d %B %Y %H::%M::%S %Z", info);
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
    _totalrespond.insert(std::pair<std::string, std::string>("Connection", _connection));
}

void HTTP::respond::set_host(std::string host)
{
    std::string host_cmp;

    host = host.substr(1, host.size() - 1);
    host_cmp = host.substr(0, host.find(':'));
    if (host_cmp != _pars_server._host)
        return (set_status_code(400));
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
        if ((it->first != "Content-Length" || it->second != "0") && (!it->second.empty()))
        {
            _totalheader.append(it->first);
            _totalheader.append(": ");
            _totalheader.append(it->second);
            _totalheader.append("\r\n");
        }
    }
    _totalheader.append("\r\n");
    _totalheader.append(_body);
}

void HTTP::respond::find_total_file_path()
{
    std::string key;
    std::string redir;
    int found;

    found = -1;
    _relativepath = _map_req["URI"].c_str();
    std::cout << "RELATIVE PATH IS[" << _relativepath  <<"]"<< std::endl;
    std::cout << "RELATIVE PATH IS[" << _relativepath[_relativepath.size() - 1]<<"]"  << std::endl;
    if (_relativepath[_relativepath.size() - 1] != '/')
        _relativepath = _relativepath.append("/");
    key = "/";
    key = key.append(_relativepath);
    while (key != "/")
    {
        std::cout << "in loop" << key << std::endl;
        key = key.substr(0, key.find_last_of('/') + 1);
        if (_pars_server._location_map.count(key) == 1)
            break;
        if (key[key.size() - 1] == '/')
            key = key.substr(0, key.size() - 1);
        
    }
     std::cout << "in HERE block" << std::endl;
    if (_pars_server._location_map[key]._redir != "")
    {
        std::cout << "in redir block" << std::endl;
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
                std::cout << RED << "AUTO INDEX" << R << std::endl;
                std::ifstream file("www/html_pages/downloads/index.php");
                if (file.is_open())
                {
                    std::cout << "in downloads" <<std::endl;
                    _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                }
                set_content_len(_body);
                _status_code = 200;
                return ;
        }
    }
    std::cout << YELLOW << "path" << _path << std::endl;
	if (stat(_path, &sb) == -1)
    {
        std::cout<< YELLOW << "file" << R << std::endl;
		return (set_status_code(404)); // file doesnt exist
    }
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
    if (_contentlen == "0" && _status_code == 0)
        _status_code = 204;
    else if (_status_code == 0)
        _status_code = 200;
    
}

const std::string &HTTP::respond::getTotalheader() const
{
    return _totalheader;
}
