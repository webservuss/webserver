#include "respond.hpp"

#include <iostream>
#include <map>

 HTTP::respond::respond(std::map < std::string, std::string> mapHeader){


    std::cout  << "st$$$$$$$$$$$$" << std::endl;

    std::string findKey;
    findKey = mapHeader["GET"];
    std::string statusline = status_line(findKey);
    std::cout << "statusline" << statusline << std::endl;

    // get respond line
    // get body
     std::map<std::string, std::string>::iterator it = mapHeader.begin();
     std::cout << RED <<  "*******************    MAP REQUEST CONTAINTS   *******************\n";
     for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
         std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
}

std::string HTTP::respond::status_line(std::string findKey){


    //std::cout << findKey << std::endl;

    int n = 0;
    std:: string retval;
    size_t pos = 0;
    int i = 0;
    int j = 0;
    char * needle = strdup("HTTP/1.1");
   // std::string h = "HTTP1.1";
   // char t = <char*>(h.t_str());
    //char *target = t;
    char * c = const_cast<char*>(findKey.c_str());
    char *res = c;
    while((res = std::strstr(res, needle)) != nullptr) {
        ++res;
        j = 2;
    }
    if(j == 2)
        return("HTTP/1.1 200 OK");
    //error
    return(" ");

}

const std::string &HTTP::respond::getStatusline() const {

    return _statusline;
}
