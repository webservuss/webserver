#include "parser_HTTP.hpp"




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
    std::istringstream request_data(dataparser);
   // std::cout << RED << "data" << dataparser << RESET << std::endl;
    while(std::getline(request_data, data))
    {
        std::cout << "data per line:" << data << std::endl;
    }
    //std::cout<std::string> data <
    std::vector<std::string> element = utils::split_http(dataparser, " ");
    for( int i = 0; i <element.size(); i++)
    {
        std::cout << "after split" << element[i] << std::endl;
    }


}

