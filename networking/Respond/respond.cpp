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
    //    int serverMaximum = _body.size();
    //    if( serverMaximum > _body.length())
    //        std::cout << " TO BIG MAXIMUM SIZE REACHED" << std::endl;
    //if (maxbodysize < _body.length()[]
    //  int fd;
   
    std::string     total_path = _totalpath;
    std::ifstream file("html_pages/welcome.php");
    std::cout << GREEN << "file :: " << file << R << std::endl;
   // std::ifstream file("html_pages/index.html");
    //fd = open(&file[0], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
       // std::ifstream file("html_pages/auto_error.html");
        std::string total_body;
        if (file.is_open())
        {
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
            std::cout << RED <<  "POST =" << _body << R <<  std::endl;
        }
    if (this->filefd == -1 && _status_code == 200)
        // this->setstatus(403);
        std::cout << "status code 403 " << std::endl;
    //struct stat statBuf;
   // if (stat(file, &statBuf) < 0 && _status == 200)
      //  std::cout << "status code 201 " << std::endl;
    // this->setstatus(201);
    std::cout << RED << "BODY =" <<_body << R <<std::endl;
    if (write(filefd, _body.c_str(),_body.length()) == -1)
        std::cout<< "WRITE " << std::endl;
    close(filefd);
    std::cout << GREEN << "BEN JE HIER  " << file << R << std::endl;
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
    
    std::ifstream file ("html_pages/auto_error.html");
    std::string total_body;
    if(root != "auto_error.html;" )
    {   
        _statusline.append("404 no 404 line ");
        _body = "<h1>404: not present in config file</h1>\0";
        set_content_len(_body);
    }
    else if(file.is_open())
            _body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}


void HTTP::respond::set_status_line()
{
    std::string total_body;
    std::cout << YELLOW << "last modified : " << _pars_server._error_page[1] << R<< std::endl;
    _statusline = "HTTP/1.1 ";
    if (_status_code == 200)
        _statusline.append("200 OK");
    // if (_pars_server._auto_index == 1)
    // {
    //     std::cout << RED << "AUTO INDEX" << R << std::endl;
    //     std::ifstream file("downloads/index.php");
    //     if (file.is_open())
    //     {
    //          total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //         _body = total_body;
    //     }
    // }

    else if (_status_code == 404)
    {
        _statusline.append("404 Not Found");
        std::ifstream file("html_pages/auto_error.html");
        if ( _pars_server._error_page[0] == "404" || _pars_server._error_page[0] == "404;")
        {
            std::string root = _pars_server._error_page[1];
            set_no_config404(root);
        }
        else if (file.is_open())
        {
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
        }
        else
        {
            std::cout << YELLOW << "404 NOT OPEN" << RESET << std::endl;
            exit(10);
        }
        file.close();
        set_content_type("text/html");
        set_content_len(_body);
    }
    else if (_status_code == 403)
    {
        _statusline.append("403 Forbidden");
        _body = "<h1>403: You can't do that!</h1>\0";
        set_content_len(_body);
    }
    else if (_status_code == 405)
    {
        _statusline.append("405 Method not Allowed");
        _body = "<h1>405: Try another method!</h1>\0";
        set_content_len(_body);
    }
    else if (_status_code == 204)
        _statusline.append("204 No Content");
    else if (_status_code == 301)
        _statusline.append("301 Moved Permanently");
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
        // _lastmodified.append("\r\n");
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
