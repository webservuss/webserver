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

    // sent satus line
   // startres(req_n_conf);

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




// void HTTP::respond::startres(std::string statusline)
// {

//     std::cout << " WE Will check what the getter is and depending on that the respond will react" << std::endl;
    
//     std::cout << "get method" << res.getmethod() <<std::
//     if( HTTP._method == "GET")
//         std::cout<< "method is this "<< std::endl;
//     if(HTTP._method == "POST" )
//         std::cout<< "method is this "<< std::endl;
//     if(_method == "DELETE" )
//         std::cout<< "method is this "<< std::endl;
// }


// void HTTP::respond::getmethod()
// {
//             //basicly do what is no been done 
// }



// void HTTP::respond::postmethod()
// {

//     //The stat() function gets status information about a 
//     //specified file and places it in the area of memory pointed to by the buf argument.

//     //If the named file is a symbolic link, stat() 
//     //resolves the symbolic link. It also returns information about the resulting file.

//     _postheader = _totalheader;
//     std::string     total_path = find_total_file_path();
//     filefd = open(total_path.c_str(), O_WRONLY | O_APPEND | O_CREAT);
//     // check if the body size is allowed otherwise status code
//     //open the file from the path 
//     // set the body for POST 
//     // use stat for the file 

// }

// void HTTP::respond::deletemethod()
// {
//     _postheader = _totalheader;

//     // check if the body size is allowed otherwise status code
//     // 
// }



//  TODO also add a bad request if we dont find HTTP/1.1 !!!
void HTTP::respond::status_line(std::string findKey)
{
    std::cout << findKey << "PRINT THIS LINE " << std::endl;
    
        _statusline = "HTTP/1.1 ";
    if (_status_code == 200)
        _statusline.append("200 OK");
    else if (_status_code == 404)
    { // error  in 404 as will make server fuck up for some reason atm
        _statusline.append("404 Not Found");
        // if (_pars_server._error_page)
        _body = "<html><head><title>404 Not found</title></head><body><h1>404 Not found</h1></body>\0";
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
    struct timeval  tv;
    time_t          t;
    struct tm       *info;
    char            buffer[64];
    
    gettimeofday(&tv, NULL);
    t = tv.tv_sec;
    info = localtime(&t);
    _date = strftime(buffer, sizeof buffer, "%a, %d %B %Y %H::%M::%S %Z" , info);
    _date = buffer;
    _totalrespond.insert(std::pair<std::string, std::string>( "Date:", _date) );
}


void ::HTTP::respond::setmodified(int fileFD )
{
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

void HTTP::respond::setconnection(std::string connection)
{
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
    _totalrespond.insert(std::pair<std::string, std::string>( "Content-Language:", _language));
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


void HTTP::respond::appendheader()
{
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

std::string HTTP::respond::find_total_file_path()
{
    std::string total_path;
    std::string get_req_line = _map_req["GET"].c_str();
    std::string pathfind = "";
    std::string resultpathfind = "";
    int i = 0;

    while (get_req_line[i] != '/')
        i++;
    pathfind = get_req_line.substr(i, get_req_line.size() - i);
    if (pathfind.find(' ') - 1 >= 1) // check not root 
    {
        resultpathfind = pathfind.substr(1, pathfind.find(' ') - 1);
        // really need to do per method now hard coded .html
        resultpathfind.append(".html");
    }
    // if root find index from config
    if (resultpathfind == "")
        resultpathfind = _pars_server._index;
    total_path = _pars_server._root.append(resultpathfind);
    return (total_path);
}

void    HTTP::respond::set_status_code(int code)
{
    _status_code = code;
}


void    HTTP::respond::setbody()
{
    std::string     total_body;
    std::string     total_path = find_total_file_path();
    const char      *_path = total_path.c_str();
    std::ifstream   file(_path);
    struct stat     sb;

    if (stat(_path, &sb) == -1)
    {
        // if (_pars_server._error_page.size() > 1)
        // {
        //     std::string err_pg_path = "";
        //     std::string err_pg_code = "";
        //     std::string total_err_path = "";
        //     std::string root;
        //     int i = 0;
        //     while (_pars_server._error_page[0][i] == ' ')
        //         i++;
        //     err_pg_code = _pars_server._error_page[0].substr(i, _pars_server._error_page[0].size() - i);
        //     i = 0;
        //     while (_pars_server._root[i] == ' ')
        //         i++;
        //     root = _pars_server._root.substr(i, _pars_server._root.size() - i);
        //     err_pg_path = _pars_server._error_page[1].substr(1, _pars_server._error_page.size() - 1);
        //     // if (err_pg_code == "404")
        //     //     err_pg_path = root.append(_pars_server._error_page[1].substr(1, _pars_server._error_page.size() - 1));
        //     std::cout << "root: [ " << root << "]"<< std::endl;
        //     std::cout << "err_pg_path " << err_pg_path << "]"<< std::endl;
        //     std::cout << "err_pg_path: [ " << err_pg_path << "]"<< std::endl;
        // }
        // // _statusline.append("404 Not Found");
        // // if (_pars_server._error_page)
        // // _body = "<html><head><title>404 Not found</title></head><body><h1>404 Not found</h1></body>\0";
        // // setContentlen(_body);
        // // file(path);
        return (set_status_code(404));   // file doesnt exist
    }
    if(file.is_open())
    {
        total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        _body = total_body;
    }
    else
        return (set_status_code(403)); // forbidden no access rights
    setContentlen(total_body);
    if (_contentlen == "0" && _status_code == 0)
        _status_code = 204;
    else if (_status_code == 0)
        _status_code = 200;
}

const std::string &HTTP::respond::getTotalheader() const
{
    return _totalheader;
}
