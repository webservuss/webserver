#include "respond.hpp"

#include <iostream>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <cstring>
#include <iterator>

HTTP::respond::respond(t_req_n_config req_n_conf)
{
    _map_req = req_n_conf._req_map;
    _pars_server = req_n_conf._parser_server;
    std::string findKey;

    findKey = _map_req["GET"];
    setDate();
    setmodified(1);
    findKey = _map_req["Connection:"];
    setconnection(findKey);
    findKey = _map_req["Host:"];
    setHost(findKey);
    findKey = _map_req["Accept-Language:"];
    setLanguage(findKey);
    setbody();
    status_line(findKey);
    appendheader();
}

//  TODO also add a bad request if we dont find HTTP/1.1 !!!
void HTTP::respond::status_line(std::string findKey)
{
    std::cout << findKey << std::endl;
    // int j = 0;
    // char * c = const_cast<char*>(findKey.c_str());
    // char *res = c;
    // while((res = std::strstr(res, "HTTP/1.1")) != nullptr) {
    //     ++res;
    //     j = 2;
    // }
    // _statusline = "";
    // if (j == 2)
        _statusline = "HTTP/1.1 ";
    if (_status_code == 200)
        _statusline.append("200 OK");
    else if (_status_code == 404)
    {
        _statusline.append("404 Not Found");
        _body = "<h1>404 Not found</h1>\0";
        setContentlen(_body);
    }
    else if (_status_code == 403)
    {    
        _statusline.append("403 Forbidden");
        _body = "<h1>403: You can't do that!</h1>\0";
        setContentlen(_body);
    }
    else if (_status_code == 204)
        _statusline.append("204 No Content");
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
        _date = strftime(buffer, sizeof buffer, "%a, %d %B %Y %H::%M::%S %Z" , info);
        _date = buffer;
        //std::cout << "date :" << _date << std::endl;
        _totalrespond.insert(std::pair<std::string, std::string>( "Date:", _date) );
}


void ::HTTP::respond::setmodified(int fileFD ){

    struct stat	stat;
    struct tm	*info;
    char		timestamp[36];

    fstat(fileFD, &stat);
    info = localtime(&stat.st_mtime);
    strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
    _lastmodified.append(timestamp);
    _lastmodified.append("\r\n");
    std::cout << "last modified : " << _lastmodified <<  std::endl;
    _totalrespond.insert(std::pair<std::string, std::string>( "Last-Modified:", _lastmodified) );
}

void HTTP::respond::setconnection(std::string connection){

    _connection = connection;
    _totalrespond.insert(std::pair<std::string, std::string>( "Connection:", _connection) );
}


void HTTP::respond::setHost(std::string host)
{
    _host = host;
    _totalrespond.insert(std::pair<std::string, std::string>( "Host:", _host) );
}

void HTTP::respond::setLanguage(std::string contentlanguage)
{
    _language = contentlanguage;
    _totalrespond.insert(std::pair<std::string, std::string>( "Content-Language:", _language) );
}


const std::string &HTTP::respond::getStatusline() const 
{
    return _statusline;
}


void HTTP::respond::setContentlen(std::string body)
{
    int size;

    size = body.size();
    std::stringstream ss;
    ss << size;
    ss>> _contentlen;
    _totalrespond.insert(std::pair<std::string, std::string>( "Content-Length:", _contentlen) );

}


void HTTP::respond::appendheader() {


    _totalheader.append(_statusline);
    _totalheader.append("\r\n");
    std::map<std::string, std::string>::iterator it = _totalrespond.begin();
    for (it = _totalrespond.begin(); it != _totalrespond.end(); ++it) 
    {
        if (it->first != "Content-Length:" || it->second != "0")
        {
            _totalheader.append(it->first);
            _totalheader.append(" ");
            _totalheader.append(it->second);
            _totalheader.append("\r\n");
        }
    }
    _totalheader.append(_body);
    std::cout << "total header is: " << _totalheader << std::endl;
}



void HTTP::respond::setbody()
{
    std::string root;
    root = _pars_server._root;
    std::cout << "ROOT[" << _pars_server._root << "]" << std::endl;
    std::cout << "get:[" << _map_req["GET"] << "]"  << std::endl;
    int i = 0;
    std::string get_req_line = _map_req["GET"].c_str();
    std::string pathfind = "";
    while (get_req_line[i] != '/')
        i++;
    pathfind = get_req_line.substr(i, get_req_line.size() - i);
    std::string resultpathfind = "";
    std::cout << "pathfind.find(' ') - 1:" << pathfind.find(' ') - 1 << std::endl;
    if (pathfind.find(' ') - 1 >= 1)
    {
        resultpathfind = pathfind.substr(1, pathfind.find(' ') - 1);
        resultpathfind.append(".html");
    } // really need to do per method now hard coded .html
    if (resultpathfind == "")
        resultpathfind = _pars_server._index;
    std::cout << " pathfind: [" << pathfind << "]" << std::endl;
    std::cout << " resultpathfind: [" << resultpathfind << "]" << std::endl;
    std::string total_path = _pars_server._root.append(resultpathfind);
    std::cout << "PATH TOTAL IS[" << total_path << "]" << std::endl;
    const char *path = total_path.c_str();
    std::ifstream file(path);
    struct stat sb;
    if (stat(path, &sb) == -1)
    {   
        std::cout << "file NO exists"<< std::endl;
        _status_code = 404;
        return;
    }
    if(file.is_open())
    {
        total_path = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        _body = total_path;
    }
    else
    {
        std::cout << "FILE IS NOT OPEN" << std::endl;
        _status_code = 403;
        return;
    }
    setContentlen(total_path);
    if (_contentlen == "0" && _status_code == 0)
    {   
        std::cout << "NO content" << std::endl;
        _status_code = 204;
    }
    else if (_status_code == 0)
    {    
        std::cout << "OKK" << std::endl;
        _status_code = 200;
    }
}

const std::string &HTTP::respond::getTotalheader() const
{
    return _totalheader;
}
