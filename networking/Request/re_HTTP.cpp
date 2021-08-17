#include "re_HTTP.hpp"

#include <iostream>
#include <map>
#include <iostream>
#include <string>


using namespace HTTP;

std::string methods[3] = {
        "GET",
        "POST",
        "DELETE",
};


const std::string re_HTTP::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";

HTTP::re_HTTP::re_HTTP(std::string dataparser)
{
    std::string data;
    std::string line;
    std::string method;
    int j = 0;

    // here set the request line and define the method.
    //getMethod(); //
    std::istringstream request_data(dataparser);
    set_headers(dataparser);
    int len ;
    int i = 0;
    len = _totalBody.length();
    std::cout << "LEN" << len << std::endl;
    while(std::getline(request_data, data ) && !request_data.eof())
    {
        line = data.substr(0, data.find('\r'));
        std::cout << RED <<"line: "<< "{"  << line << "}"<< std::endl;
        if (i == 0) 
        {
            setRequestline(line);
            mapHeader.insert(std::pair<std::string, std::string>( "METHOD", _method) );
            mapHeader.insert(std::pair<std::string, std::string>( "URI", _uri) );
            mapHeader.insert(std::pair<std::string, std::string>( "PROTOCOL", _protocol) );
            i++;
        }
        std::cout  << RED << "HERE 1 "<< std::endl;
        if(line.size() != 0)
            split_line(line);
        mapHeader.insert(std::pair<std::string, std::string>( _key, _value) );
        std::cout << "j" << j <<std::endl;
        j++;
        }

    std::cout  << RED << "HERE 2 "<< std::endl;
    std::map<std::string, std::string>::iterator it = mapHeader.begin();
    std::cout << RED <<  "*******************MAP REQUEST CONTAINTS*******************\n";
    for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
}


void HTTP::re_HTTP::set_headers(std::string header) {

   // std::string _totalBody;
    int i = 0;
    while (header[i] != '\r')
        i++;
    std::cout << "i" << i << std::endl;
    _totalBody = header.substr(i, header.size() - i);
    std::cout << "TOTAL BODY " << _totalBody << std::endl;
}

void HTTP::re_HTTP::set_key(std::string key)
{
            _key = key;
           
}

void HTTP::re_HTTP::set_value(std::string value)
{
            _value = value;
         
}

void HTTP::re_HTTP::split_line(std::string line){

    set_key(line.substr(0, line.find(" ")));
    set_value(line.substr(line.find(" "), line.find('\r')));


}


std::string &re_HTTP::getMethod() {
//TODO method needs to checked if same as config otherwise (405)
    //const char* method;
    // loop through the string of methods and compare it with the set method out of the requestline.
    for (int i = 0; i < 3; i++){
        if (_method.compare(methods[i]) == 0)
            return methods[i];
    }
    if (getMethod().empty()) {
        std::cout << " ERROR: _status = 400" << std::endl;
    }
    int i = 0;
    return methods[i];
    std::cout << YELLOW << "method" << _method << R << std::endl;
}

void re_HTTP::setMethod(const std::string &method) {
    _method = method;
}

const std::string &re_HTTP::getRequestline() const {
    return _requestline;
}

void re_HTTP::setRequestline(std::string &requestline) {

    
    int found = -1;
    for(int i = 0; i < 3; i++)
    {
        std::cout << YELLOW<< "i" << i << R << std::endl;
        found = requestline.find(methods[i]);
        std::cout << RED << "FOUND" << found << std::endl;
        std::cout << GREEN << methods[i] << " method" << RESET <<  std::endl;
        if(found != -1)
        {
            std::cout << YELLOW " HERE" << R << std::endl;
            _method = requestline.substr(found, methods[i].size());
            std::string tmp = requestline.substr(_method.size() + 1, requestline.size()- methods[i].size());
            _uri = tmp.substr(0, tmp.find(' '));
            _uri = _uri.substr(1, _uri.size() - 1);
            std::cout << "_method : [" <<  _method << "]" << std::endl;
            std::cout << "_uri : [" <<  _uri << "]" << std::endl;
            _protocol = tmp.substr(_uri.size() + 1, tmp.size() - _uri.size());
            std::cout << "_protocol : [" <<  _protocol << "]" << std::endl;
            break;
        }
    }

    if(found == -1)
    {
        std::cout << "NO METHOD" << std::endl; // error message.
        _method = "";
        _uri = "";
        _protocol = "";
    }
    // _requestline = requestline;
    // std::string lin = requestline.c_str();
}


//get URI
// const std::string &re_HTTP::getUri() const {
//     std::cout << YELLOW << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<< _uri << R <<  std::endl;
//     return _uri;
// }

//define URI
// void re_HTTP::setUri(const std::string &uri) {
//     _uri = uri;
// }

HTTP::re_HTTP::~re_HTTP()
{
    std::cout << "destructor" << std::endl;
    for (std::map<std::string, std::string>::iterator itft = mapHeader.begin(); itft!=mapHeader.end(); ++itft)
        std::cout << itft->first << " => " << itft->second << '\n';
}
