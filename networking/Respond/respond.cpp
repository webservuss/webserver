#include "respond.hpp"
#include <sys/stat.h>

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
    // else if (_map_req["METHOD"].compare("POST") == 0)
    //     postmethod();
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
    setDate();
    setmodified();
    setconnection(_map_req["Connection"]);
    setHost(_map_req["Host"]);
    setLanguage(_map_req["Accept-Language"]);
    setbody();
    set_status_line();
    set_total_response();
}





void HTTP::respond::deletemethod()
{
    // _postheader = _totalheader;
    // int ret  = remove(file.c_str());
    // if( ret != 0)
    // set statuscode notfound

}

void HTTP::respond::set_no_config404(std::string root)
{
    if(root != "error_page.html;" )
    {   
        std::cout << YELLOW << "THER IS NO CONFIG" << R << std::endl;
        _statusline.append(_stat_cha);
        _body.append(_stat_cha);
        _body = "<h1>status code is not present in config file</h1>\0";
        std::cout << _body  << "BODY"<< std::endl;
        setContentlen(_body);
    }
}


void HTTP::respond::set_status_line()
{
    std::cout <<"ERROR"<< std::endl;
    std::string tmp = std::to_string(_status_code);
    _stat_cha = tmp.c_str();
    std::string total_body;
    std::string root = _pars_server._error_page[1];
    std::ifstream file("html_pages/auto_error.html");
    _statusline = "HTTP/1.1 ";
    std::string _stat_cha_s = _stat_cha;
    _stat_cha_s.append(";");
    if ( _pars_server._error_page[0] == _stat_cha || _pars_server._error_page[0] == _stat_cha_s )
         set_no_config404(root);
    else if (_status_code == 404)
    {
        std::cout <<"ERROR"<< std::endl;
         _statusline.append("404 Not Found");
        _body = "<h1>404: You can't do that!</h1>\0";
    }
    else if (_status_code == 200)
        _statusline.append("200 OK");
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
        _statusline.append("204 No Content");
    else if (_status_code == 301)
        _statusline.append("301 Moved Permanently");
    else if(_status_code == 400)
    {
        _statusline.append("400 ");
        _body = "<h1> 400 :Bad Request The request could not be understood by the server due to malformed syntax. The client SHOULD NOT repeat the request without modifications. </h1> \0";
    }
    else if(_status_code == 413)
    {
        _statusline.append("113");
        _body = "<h1> 400 : Request Entity Too Large \0";
    }
    else if (file.is_open())
        {
            std::cout << " THERE IS A CONFIG " << std::endl;
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
        }
    file.close();
    setcontenttype("text/html");
    setContentlen(_body);
}

void HTTP::respond::setcontenttype(const std::string &contentype)
{
    _contentype = contentype;
    _totalrespond.insert(std::pair<std::string, std::string>("Content-Type", _contentype));
}

void HTTP::respond::setDate()
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

void ::HTTP::respond::setmodified()
{
    struct stat stats;
    struct tm *info;
    char timestamp[1000];

    if (stat(_totalpath.c_str(), &stats) == 0)
    {
        info = gmtime(&stats.st_mtime);
        strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
        _lastmodified = timestamp;
        // _lastmodified.append("\r\n");
    }
    _totalrespond.insert(std::pair<std::string, std::string>("Last-Modified", _lastmodified));
}

void HTTP::respond::setconnection(std::string connection)
{
    _connection = connection;
    _totalrespond.insert(std::pair<std::string, std::string>("Connection", _connection));
}

void HTTP::respond::setHost(std::string host)
{
    _host = host;
    _totalrespond.insert(std::pair<std::string, std::string>("Host", _host));
}

void HTTP::respond::setLanguage(std::string contentlanguage)
{
    _language = contentlanguage;
    _totalrespond.insert(std::pair<std::string, std::string>("Content-Language", _language));
}

void HTTP::respond::setContentlen(std::string body)
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

    _relativepath = _map_req["URI"].c_str();
    std::cout << "RELATIVE PATH IS[" << _relativepath  <<"]"<< std::endl;
    std::cout << "RELATIVE PATH IS[" << _relativepath[_relativepath.size() - 1]<<"]"  << std::endl;
    if (_relativepath[_relativepath.size() - 1] != '/')
        _relativepath = _relativepath.append("/");
    key = "/";
    key = key.append(_relativepath);
    std::cout << "RELATIVE PATH IS[" << _relativepath  <<"]"<< std::endl;
    // if (_pars_server._auto_index == 1)
    // {
    //     // check if its a directory. 
    //     struct stat buffer;
    //     int ret;

    //     ret = stat(_relativepath, &buffer);
    //     std::cout << YELLOW << "RET " << ret << std::endl;
        
    //     std::cout << RED << "AUTO INDEX" << R << std::endl;
    //     std::ifstream file("downloads/index.php");
    //     if (file.is_open())
    //          _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // }
    if (_pars_server._auto_index == 1)
    {
        // check if its a directory. 
        struct stat buffer;
        int ret;

        ret = lstat(_relativepath.c_str(), &buffer);
        std::cout << YELLOW << "RET " << ret << std::endl;
        if(ret == 0)
        {
            std::cout << RED << "AUTO INDEX" << R << std::endl;
            std::ifstream file("downloads/index.php");
            if (file.is_open())
             _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        }
    }
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
        std::cout << "in method block" << std::endl;

        if (_map_req["METHOD"].compare(_pars_server._location_map[key]._method) != 0)
            return (set_status_code(405));
        else
        {
            if (_relativepath == "" || _relativepath == "/")
                _relativepath = _pars_server._location_map[key]._index;
            _totalpath = _pars_server._location_map[key]._root.append(_relativepath);
            std::cout << "relative path is end[" << _relativepath << "]" << std::endl;
            std::cout << "_totalpath path is end[" << _totalpath << "]" << std::endl;
        }
    // if (_pars_server._auto_index == 1)
    // {
    //     // check if its a directory. 
    //     struct stat buffer;
    //     int ret;

    //     ret = stat(_relativepath.c_str(), &buffer);
    //     std::cout << YELLOW << "RET " << ret << std::endl;
    //     if(ret == 0)
    //     {
    //         std::cout << RED << "AUTO INDEX" << R << std::endl;
    //         std::ifstream file("downloads/index.php");
    //         if (file.is_open())
    //          _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //     }
    // }
    }

    return ;
}

void HTTP::respond::set_status_code(int code)
{
    _status_code = code;
    return;
}

void HTTP::respond::setbody()
{
    const char      *_path;
    struct stat     sb;

    if (_status_code == 405)
        return;
    _path = _totalpath.c_str();
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
			return (set_status_code(403)); // forbidden no access rights
		file.close();
	}
    setContentlen(_body);
    if (_contentlen == "0" && _status_code == 0)
        _status_code = 204;
    else if (_status_code == 0)
        _status_code = 200;
}

const std::string &HTTP::respond::getTotalheader() const
{
    return _totalheader;
}
