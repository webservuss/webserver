#include "parser_HTTP.hpp"

#include <iostream>
#include <map>


using namespace HDE;



const std::string parser_HTTP::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";

const std::map<std::string, method> parser_HTTP::mapMethod = 
UTILS::makeMap<std::string, method>
("GET", GET)
("POST", POST)
("DELETE", DELETE);

HDE::parser_HTTP::parser_HTTP(std::string dataparser)
{
    std::string data;
    std::string line;

    std::istringstream request_data(dataparser);
    while(std::getline(request_data, data))
    {
        line = data.substr(0, data.find('\r'));
        if(line.size() != 0)
            split_line(line);
        
        mapHeader.insert(std::pair<std::string, std::string>( _key, _value) );
    }
    std::map<std::string, std::string>::iterator it = mapHeader.begin();
    std::cout << RED <<  "*******************    MAP HEADER CONTAINTS   *******************\n";
    for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
}


void HDE::parser_HTTP::set_key(std::string key)
{
            _key = key;
           
}

void HDE::parser_HTTP::set_value(std::string value)
{
            _value = value;
         
}

void HDE::parser_HTTP::split_line(std::string line){

    set_key(line.substr(0, line.find(" ")));
    set_value(line.substr(line.find(" "), line.find('\r')));


}
