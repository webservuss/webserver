#include "parser_config.hpp"
#include "colors.hpp"

#include <iostream>
#include <map>
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <map>


using namespace HDE;


const std::string parser_config::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";


HDE::parser_config::parser_config(std::ifstream &file)
{
    std::cout<< RED<< "###########WE ARE IN PARSER CONFIG##########" << RESET << std::endl;
    std::string line;

  
    if(file)
        std::cout<< RED<< "file is open lets read" << RESET << std::endl;
    std:size_t len = line.size();
    while(std::getline(file,line))
    {
       
        line = line.substr(0, line.find(";"));
        if(line.size()!= 0)
            split_config(line);
       // mapconfig.insert(std::pair<std::string, std::string>( _key, _value) );
        
    }
    std::map<std::string, std::string>::iterator it = mapconfig.begin();
    std::cout << RED <<  "*******************    MAP HEADER CONTAINTS   *******************\n";
    for (it=mapconfig.begin(); it!=mapconfig.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
   
}
  
 
void        HDE::parser_config::split_config(std::string line)
{

    std::string key;
    std::string value;
    
    set_key(line.substr(0, line.find(" ")),line.substr(line.find(" "), line.find('\r')));
    set_value(line.substr(line.find(" "), line.find('\r')));
    
    
    

}



void HDE::parser_config::set_key(std::string key, std::string value)
{
            _key = key;
            std::cout << " key " << key << std::endl;
            if(key == "server_name")
                set_server_name(key, value);
           
}

void HDE::parser_config::set_value(std::string value)
{
            _value = value;
            std::cout << " value " << value << std::endl;
         
}



void		HDE::parser_config::set_server_name(std::string server, std::string server_name)
{

    _server = server_name;
    _server_name = server_name;
    std::cout << " HIER" << _server_name << std::endl;
    mapconfig.insert(std::pair<std::string, std::string>( _server, server_name));
}

//listen
void		HDE::parser_config::set_port(int port)
{
    _port = port;
}

void		HDE::parser_config::set_host(std::string host)
{
    host = _host;
}

void		HDE::parser_config::set_error_page(std::string error_page)
{
    _error_page = error_page;
}

void		HDE::parser_config::set_auto_index(int auto_indx)
{
    _auto_index = auto_indx;
}

void 		HDE::parser_config::set_root(std::string root)
{
    _root = root;
}

void 		HDE::parser_config::set_index(std::string indx)
{
    _index = indx;
}










// }









    // while(std::getline(request_data, data))
    // {
    //     line = data.substr(0, data.find('\r'));
    //     if(line.size() != 0)
    //         split_line(line);
        
    //     mapConfig.insert(std::pair<std::string, std::string>( _key, _value) );
    // }
    // std::map<std::string, std::string>::iterator it = mapConfig.begin();
    // std::cout << RED <<  "*******************    MAP HEADER CONTAINTS   *******************\n";
    // for (it=mapConfig.begin(); it!=mapConfig.end(); ++it)
    // std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';


// void HDE::parser_config::parser_config_open(){

//     std::cout<< RED<< "#LALA#" << RESET << std::endl;

// }