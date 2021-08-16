#include "respond.hpp"

#include <iostream>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <cstring>
#include <iterator>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <sstream>
// TODO :
// check if its redirection -> if in de config file url location block is a redirection 1 change it to redirection
// and then continue.

//yes also notes, we need to implement the following status codes: for redirect 301 (moved permenantely),

//307 (temporary redirect) and 405(method not allowed e.g. if method not same as config) and

//413 (request entity is larger than limits defined by server
//(I think this is what you are talking about)) after that status codes should be done

HTTP::respond::respond(t_req_n_config req_n_conf)
{
    _status_code = 0;
    _map_req = req_n_conf._req_map;
    _pars_server = req_n_conf._parser_server;
    std::string findKey;

    startres();
    setDate();
    setmodified(1);
    findKey = _map_req["Connection:"];
    std::cout << "KEY" << findKey << std::endl;
    setconnection(findKey);
    findKey = _map_req["Host:"];
    setHost(findKey);
    findKey = _map_req["Accept-Language:"];
    setLanguage(findKey);
    setbody();
     std::cout <<  "BODY>?" << _body << std::endl;
    status_line(findKey);
    appendheader();
}

HTTP::respond::~respond()
{
}

void HTTP::respond::startres()
{
    if (_map_req.count("GET") > 0)
        setbody();
    if (_map_req.count("POST") > 0)
        return postmethod();
    if (_map_req.count("DELETE") > 0)
        deletemethod();
    else
        std::cout << " the method is nog right" << std::endl;
}

void HTTP::respond::postmethod()
{
    // if (bodysize < contentlen())
       // std::cout << " check the size" << std::endl;
    std::ifstream file("html_pages/index.html");
    std::cout << GREEN << "file :: " << file << R << std::endl;
        std::string total_body;
        if (file.is_open())
        {
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
            std::cout << RED <<  "POST :" << _body << R <<  std::endl;
        }
    // if (file == -1 && _status == 200)
    //     std::cout << "status code 403 " << std::endl;
    // std::cout << RED << "BODY :" <<_body << R <<std::endl;
    // if (write(file, _body.c_str(),_body.length()) == -1)
    //     std::cout<< "WRITE " << std::endl;
    // close(file);
    
}

void HTTP::respond::deletemethod()
{
    _postheader = _totalheader;
    int ret  = remove(_deletefile.c_str());
    if( ret != 0)
        std::cout << "not found" << std::endl;

}

//  TODO also add a bad request if we dont find HTTP/1.1 !!!
void HTTP::respond::status_line(std::string findKey)
{
    std::cout << RED << findKey << "PRINT THIS LINE " << R<<std::endl;

    _statusline = "HTTP/1.1 ";
    if (_status_code == 200)
        _statusline.append("200 OK");
    else if (_status_code == 404)
    { // error  in 404 as will make server fuck up for some reason atm
        _statusline.append("404 Not Found");
        // if (_pars_server._error_page)
        //_body = "<html><head><title>404 Not found</title></head><body><h1>404 Not found</h1></body></html>";
        std::ifstream file("html_pages/auto_error.html");
        std::string total_body;
        if (file.is_open())
        {
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
            std::cout << RED <<  "BODYYYYYY" << _body << R <<  std::endl;
        }
        else
        {
            // TODO throw exception
            std::cout << YELLOW << "404 NOT OPEN" << RESET << std::endl;
            exit(10);
        }
        file.close();
        setcontenttype("text/html");
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

void HTTP::respond::setcontenttype(const std::string &contentype)
{
    _contentype = contentype;
    _totalrespond.insert(std::pair<std::string, std::string>("Content-Type:", _contentype));
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
    _totalrespond.insert(std::pair<std::string, std::string>("Date:", _date));
}

void ::HTTP::respond::setmodified(int fileFD)
{
    struct stat stat;
    struct tm *info;
    char timestamp[36];

    fstat(fileFD, &stat);
    info = localtime(&stat.st_mtime);
    strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
    _lastmodified.append(timestamp);
    _lastmodified.append("\r\n");
    //std::cout << "last modified : " << _lastmodified << std::endl;
    _totalrespond.insert(std::pair<std::string, std::string>("Last-Modified:", _lastmodified));
}

void HTTP::respond::setconnection(std::string connection)
{
    _connection = connection;
    _totalrespond.insert(std::pair<std::string, std::string>("Connection:", _connection));
}

void HTTP::respond::setHost(std::string host)
{
    _host = host;
    _totalrespond.insert(std::pair<std::string, std::string>("Host:", _host));
}

void HTTP::respond::setLanguage(std::string contentlanguage)
{
    _language = contentlanguage;
    _totalrespond.insert(std::pair<std::string, std::string>("Content-Language:", _language));
}

void HTTP::respond::setContentlen(std::string body)
{
    int size;

    size = body.size();
    std::stringstream ss;
    ss << size;
    ss >> _contentlen;
    _totalrespond.insert(std::pair<std::string, std::string>("Content-Length:", _contentlen));
}

void HTTP::respond::appendheader()
{
    _totalheader.append(_statusline);
    _totalheader.append("\r\n");
    std::map<std::string, std::string>::iterator it = _totalrespond.begin();
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

    _totalheader.append(_body);
    std::cout << RED << "total header is: " << RESET << _totalheader << std::endl;
}

// add the root to the path . and maybe append html maybe... 
std::string HTTP::respond::find_total_file_path()
{
    std::string total_path;
   
    std::string get_req_line = _map_req["GET"].c_str();
    std::string get_reg_line;
    std::string pathfind = "";
    std::string resultpathfind = "";
    int i = 0;
 
    while (get_req_line[i] != '/' )
        i++;
    // if (_map_req.count("GET") > 0)
    // {
    //     std::cout << "pathfind for GET" << pathfind << std::endl;
    //     pathfind = get_req_line.substr(i, get_req_line.size() - i);
    // }
    pathfind = get_req_line.substr(i, get_req_line.size() - i);
    if (pathfind.find(' ') - 1 >= 1) // check not root
    {
        std::cout << "9" << std::endl;
        resultpathfind = pathfind.substr(1, pathfind.find(' ') - 1);
        // TODO really need to do per method now hard coded .html. Uncomment this to show images for index.html.
        //resultpathfind.append(".html");
    }
   // if root find index from config
    if (resultpathfind == "")
        resultpathfind = _pars_server._index;
    total_path = _pars_server._root.append(resultpathfind);
    std::cout << RED << "get_req_line: [" << get_req_line << "]" << R << std::endl;
    std::cout << "pathfind: [" << pathfind << "]" << std::endl;
    std::cout << "resultpathfind: [" << resultpathfind << "]" << std::endl;
    std::cout << "_pars_server._root:[" << _pars_server._root << "]" << std::endl;
    std::cout << "12" << std::endl;
    return (total_path);
}

void HTTP::respond::set_status_code(int code)
{
    _status_code = code;
}

void HTTP::respond::setbody()
{
    
    if (_map_req.count("POST") < 0)
        return;
    std::string total_body;
    std::string total_path = find_total_file_path();
    const char *_path = total_path.c_str();
    struct stat sb;
    std::cout << RED << "00" << RESET << std::endl;
    if (stat(_path, &sb) == -1)
    {
        std::cout << RED << "1" << RESET << std::endl;
        return (set_status_code(404)); // file doesnt exist
    }

    // if (total_path.find(".php") != std::string::npos)
    // {
    //     std::cout << RED << "2" << RESET << std::endl;
    //    // cgi_php();
    // }
    else
    {
        std::cout << RED << "3" << RESET << std::endl;
        std::ifstream file(_path);
        if (file.is_open())
        {
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
        }
        else
        {
            file.close();
            return (set_status_code(403));
        }
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
