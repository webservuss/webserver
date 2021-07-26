#ifndef respond_hpp
#define respond_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>


#include "../../http.hpp"
#include "../Request/re_HTTP.hpp"
// #include "../Servers/simple_server.hpp"
#include "../colors.hpp"



namespace HTTP{

    /* make an enum for GET DELETE POST */
//    enum method {
//
//        DELETE,
//        GET,
//        POST
//
//    };


    class respond{

    private:
        std::string _statusline;
    public:
        const std::string &getStatusline() const;

    public:
        respond(std::map < std::string, std::string >);
        std::string status_line(std::string getkey);


    };
}


#endif


