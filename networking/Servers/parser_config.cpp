#include "parser_config.hpp"
#include "colors.hpp"

#include <iostream>
#include <map>
#include <fstream>


using namespace HDE;


const std::string parser_config::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";


HDE::parser_config::parser_config(std::ifstream& file)
{
    std::cout<< RED<< "###########WE ARE IN PARSER CONFIG##########" << RESET << std::endl;
    std::string data;
    std::string line;
    std::ifstream config;

    
    if(config)
        std::cout<< RED<< "file is full" << RESET << std::endl;
    config.open("example.txt", std::ifstream::in);
    while(config.is_open()){
        std::cout<< RED<< "yes" << RESET << std::endl;
    if (!config) {
        std::cout << "Unable to open file config.txt";
        //exit(1);   // call system to stop
    }

    if(file)
    {
        std::cout<< RED<< "file is full" << RESET << std::endl;
    }

    while(getline(file, line))
    {
         std::cout<< RED<< "GET LINES" << RESET << std::endl;
        std::cout << line << '\n';
    }
    
 















//}









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
}

void HDE::parser_config::parser_config_open(){

    std::cout<< RED<< "#LALA#" << RESET << std::endl;

}