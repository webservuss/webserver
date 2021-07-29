#include "respond.hpp"

#include <iostream>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <cstring>
#include <iterator>


 HTTP::respond::respond(std::map < std::string, std::string > mapHeader){

    std::string findKey;
    findKey = mapHeader["GET"];
     _statusline = status_line(findKey);
     if(_statusline == " ")
         std::cout<< " BAD REQUEST" << std::endl;
    setDate();
    setmodified(1);
    findKey = mapHeader["Connection:"];
    setconnection(findKey);
    findKey = mapHeader["Host:"];
    setHost(findKey);
    findKey = mapHeader["Accept-Language:"];
    setLanguage(findKey);
    setbody();
    appendheader();
}

//  TODO also add a bad request if we dont find HTTP/1.1 !!!
std::string HTTP::respond::status_line(std::string findKey){

    std::cout << findKey << std::endl;
    int j = 0;
    char * needle = strdup("HTTP/1.1");
    char * c = const_cast<char*>(findKey.c_str());
    char *res = c;
    while((res = std::strstr(res, needle)) != NULL) {
        ++res;
        j = 2;
    }
    if(j == 2)
        return("HTTP/1.1 200 OK");
    return(" ");
}

void HTTP::respond::setDate(){

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


void HTTP::respond::setHost(std::string host){

    _host = host;
    _totalrespond.insert(std::pair<std::string, std::string>( "Host:", _host) );
}

void HTTP::respond::setLanguage(std::string contentlanguage){

    _language = contentlanguage;
    _totalrespond.insert(std::pair<std::string, std::string>( "Content-Language:", _language) );
}


const std::string &HTTP::respond::getStatusline() const {

    return _statusline;
}


void HTTP::respond::setContentlen(std::string body){

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
    for (it = _totalrespond.begin(); it != _totalrespond.end(); ++it) {

        _totalheader.append(it->first);
        _totalheader.append(" ");
        _totalheader.append(it->second);
        _totalheader.append("\r\n");
    }
    // _totalheader.append("\r\n");
    _totalheader.append(_body);
}



void HTTP::respond::setbody(){

    std::string s;
    std::ifstream file;
    const char *path = "networking/Respond/amber.html";
    file.open(path);
    if(file.is_open()) {
        s = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        _body = s;
    }
    else
        std::cout << " NOT OPEN RED " << std::endl;
    setContentlen(s);
}

const std::string &HTTP::respond::getTotalheader() const {
    return _totalheader;
}


