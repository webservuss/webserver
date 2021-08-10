#include "respond.hpp"

#include <iostream>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <cstring>
#include <iterator>


// TODO :
// check if its redirection -> if in de config file url location block is a redirection 1 change it to redirection
// and then continue.

//yes also notes, we need to implement the following status codes: for redirect 301 (moved permenantely),

//307 (temporary redirect) and 405(method not allowed e.g. if method not same as config) and


//413 (request entity is larger than limits defined by server
//(I think this is what you are talking about)) after that status codes should be done


HTTP::respond::respond(t_req_n_config req_n_conf)
{
    _map_req = req_n_conf._req_map;
    _pars_server = req_n_conf._parser_server;
    std::string findKey;

    // see if its redirection if it is

    // sent satus line
    startres();

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




void HTTP::respond::startres() {
    std::cout << " WE Will check what the getter is and depending on that the respond will react" << std::endl;
    std::cout << "_map_req[Host:];" << _map_req["URI:"] << std::endl;
    // check if method  is same as in config file. if not its 405 method not allowed  wrong code so if there differen methods.
    // limits 414 request entity
    // check if the methods are allowed?

    if (_map_req.count("GET") > 0) {
        std::cout << "GET " << std::endl;

        //getmethod();
    }
    if (_map_req.count("POST") > 0)
    {
        std::cout << "POST" << std::endl;
        postmethod();
    }

     if(_map_req.count("DELETE")>0) {

         std::cout << "DELETE " << std::endl;
         //deletemethod();
     }
     else
         std::cout << " the method is nog right" << std::endl;
   // postmethod();
}


void HTTP::respond::getmethod()
{
            // so this should be just a
            //basicly do what is no been done 
}



void HTTP::respond::postmethod()
{
    std::cout << "ik ben in en post method" << std::endl;

    // NOTE The stat() function gets status information about a specified file and places it in the area of memory pointed to by the buf argument.

    //If the named file is a symbolic link, stat() 
    //resolves the symbolic link. It also returns information about the resulting file.
    std::cout << _body << std::endl;
//    int serverMaximum = _body.size();
//    if( serverMaximum > _body.length())
//        std::cout << " TO BIG MAXIMUM SIZE REACHED" << std::endl;

    _postheader = _totalheader;
    //std::string     total_path = find_total_file_path();
    std::string total_path = "./index.html";
    filefd = open(total_path.c_str(), O_WRONLY | O_APPEND | O_CREAT);
    if(this->filefd == -1 && _status == 200)
       // this->setstatus(403);
        std::cout << "status code 403 " << std::endl;
    struct stat statBuf;
    if(stat(total_path.c_str(), &statBuf) < 0 && _status == 200)
        std::cout << "status code 201 " << std::endl;
    // this->setstatus(201);
    
    // check if the body size is allowed otherwise status code
    // open the file from the path
    // set the body for POST 
    // use stat for the file 

}

/*The DELETE method requests that
the origin server delete the resource
 identified by the Request-URI. This method
 MAY be overridden by human intervention
 (or other means) on the origin server.
 The client cannot be guaranteed that the
 operation has been carried out, even if
 the status code returned from the origin
 server indicates that the action has been
 completed successfully. However, the server
 SHOULD NOT indicate success unless, at the
 time the response is given, it intends to
 delete the resource or move it to an inaccessible
 location.

//A successful response SHOULD be
200 (OK) if the response includes an
entity describing the status, 202 (Accepted)
 if the action has not yet been enacted, or
 204 (No Content) if the action has been enacted
 but the response does not include an entity. */

//If the request passes through a
// cache and the Request-URI identifies
// one or more currently cached entities,
// those entries SHOULD be treated as stale.
// Responses to this method are not cacheable.




void HTTP::respond::deletemethod()
{
    _postheader = _totalheader;
   // int ret  = remove(file.c_str());
     // if( ret != 0)
          // set statuscode notfound
    // what happens in deletemethod ?
    // check if the body size is allowed otherwise status code
    // 
}



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


//const std::string &HTTP::respond::getStatusline() const
//{
//    return _statusline;
//}


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
    std::cout << "get_req_line: [" << get_req_line << "]"<< std::endl;
    std::cout << "pathfind: [" << pathfind << "]"<< std::endl;
    std::cout << "resultpathfind: [" << resultpathfind << "]"<< std::endl;
    std::cout << "_pars_server._root:[" << _pars_server._root  << "]"<< std::endl;
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
