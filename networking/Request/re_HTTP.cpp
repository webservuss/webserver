#include "re_HTTP.hpp"

#include <iostream>
#include <map>


using namespace HTTP;

std::string methods[4] = {
        "GET",
        "POST",
        "DELETE",
}


const std::string re_HTTP::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";

HTTP::re_HTTP::re_HTTP(std::string dataparser)
{
    std::string data;
    std::string line;
    int i = 0;

    std::istringstream request_data(dataparser);
    set_headers(dataparser);
    while(std::getline(request_data, data))
    {
        line = data.substr(0, data.find('\r'));
        i++;

        if(line.size() != 0)
            split_line(line);
        
        mapHeader.insert(std::pair<std::string, std::string>( _key, _value) );
    }
}

void HTTP::re_HTTP::set_headers(std::string header){

    std::string totalBody;
    int i = 0;
    while(header[i] != '\r')
        i++;
    std::cout << "i"  << i << std::endl;
    totalBody = header.substr(i, header.size()- i);
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
