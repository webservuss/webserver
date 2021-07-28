#include "re_HTTP.hpp"

#include <iostream>
#include <map>


using namespace HTTP;



const std::string re_HTTP::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";



// safe all the variables
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
    std::map<std::string, std::string>::iterator it = mapHeader.begin();
    std::cout << RED <<  "*******************    MAP REQUEST CONTAINTS   *******************\n";
    for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
}

void HTTP::re_HTTP::set_headers(std::string header){

    std::string totalBody;
    int i;
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
