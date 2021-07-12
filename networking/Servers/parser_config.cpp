#include "parser_config.hpp"

using namespace HDE;


const std::string parser_config::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";


HDE::parser_config::parser_config(std::ifstream &file)
{
    std::cout<< RED<< "###########WE ARE IN PARSER CONFIG##########" << RESET << std::endl;
    std::string line;
    int i= 0;
    int j=0;

  
    if(file)
        std::cout<< RED<< "file is open lets read" << RESET << std::endl;
    std:size_t len = line.size();
    //server part
    while(std::getline(file,line) && i != 8)
    {
       // we split the lines 
        if(line.find("{") || line.find("}"))
            i++;
        line = line.substr(0, line.find(";"));
        
        if(line.size()!= 0)
            split_config(line);
        j++;

       mapconfig.insert(std::pair<std::string, std::string>( _key, _value) );
        
    }
     std::cout << " j " << j << std::endl;
    std::map<std::string, std::string>::iterator it = mapconfig.begin();
    std::cout << RED <<  "*******************    MAP CONFIG CONTAINTS  *******************\n";
    for (it=mapconfig.begin(); it!=mapconfig.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
   
}
  
 
void        HDE::parser_config::split_config(std::string line)
{

    std::string key;
    std::string value;
    int i = 0;
    i++;
    set_key(line.substr(0, line.find(" ")));
    set_value(line.substr(line.find(" "), line.find('\r')));
 
    
    
    

}



void HDE::parser_config::set_key(std::string key)
{
            _key = key;
            std::cout << " key " << key << std::endl;
           // if(key == "server_name")
               // set_server_name(key, value);
           
}

void HDE::parser_config::set_value(std::string value)
{
            _value = value;
            std::cout << " value " << value << std::endl;
         
}



void		HDE::parser_config::set_server_name(std::string server_name)
{

    _server_name = server_name;
   // std::cout << " HIER" << _server_name << std::endl;
    //mapconfig.insert(std::pair<std::string, std::string>( _server, server_name));
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